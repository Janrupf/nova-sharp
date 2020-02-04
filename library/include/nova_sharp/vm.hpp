#pragma once

#include <cstdint>
#include <functional>

#include "nova_sharp/engine.hpp"

namespace nova::sharp {
    class NovaSharpVM {
    private:
        void* vm_handle;
        uint32_t domain_id;
        CoreCLRFunctions functions{};

        Result<void*> create_callable_ptr(const std::string& assembly_name,
                                          const std::string& full_qualified_type,
                                          const std::string& method);

    public:
        explicit NovaSharpVM(void* vm_handle, uint32_t domain_id, CoreCLRFunctions functions);
        ~NovaSharpVM();

        NovaSharpVM(const NovaSharpVM& other) = delete;
        NovaSharpVM(NovaSharpVM&& other) noexcept;

        NovaSharpVM& operator=(const NovaSharpVM& other) = delete;
        NovaSharpVM& operator=(NovaSharpVM&& other) noexcept = delete;

        void shutdown();
        int shutdown_with_exit_code();

        Result<uint32_t> execute(const std::string &assembly_name, const std::vector<std::string> &args);

        template <typename Fn>
        Result<std::function<Fn>> create_callable(const std::string& assembly_name,
                                                  const std::string& full_qualified_type,
                                                  const std::string& method) {
            using FnPointerT = Fn*;

            auto callable_ptr_result = create_callable_ptr(assembly_name, full_qualified_type, method);
            if(!callable_ptr_result) {
                return callable_ptr_result.convert_failed<std::function<Fn>>();
            } else {
                return Result<std::function<Fn>>::ok(std::function<Fn>(reinterpret_cast<FnPointerT>(callable_ptr_result.value())));
            }
        }
    };
} // namespace nova::sharp
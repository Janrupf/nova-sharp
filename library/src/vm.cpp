#include "nova_sharp/vm.hpp"

namespace nova::sharp {
    NovaSharpVM::NovaSharpVM(void* vm_handle, uint32_t domain_id, CoreCLRFunctions functions)
        : vm_handle(vm_handle), domain_id(domain_id), functions(functions) {}

    NovaSharpVM::NovaSharpVM(NovaSharpVM&& other) noexcept
        : vm_handle(other.vm_handle), domain_id(other.domain_id), functions(other.functions) {
        other.vm_handle = nullptr;
        other.functions = {nullptr, nullptr, nullptr, nullptr, nullptr};
    }

    NovaSharpVM::~NovaSharpVM() {
        if(vm_handle) {
            shutdown();
        }
    }
    void NovaSharpVM::shutdown() {
        functions.shutdown(vm_handle, domain_id);
        functions = {nullptr, nullptr, nullptr, nullptr, nullptr};
        vm_handle = nullptr;
    }

    int NovaSharpVM::shutdown_with_exit_code() {
        int exit_code;
        functions.shutdown_2(vm_handle, domain_id, &exit_code);
        functions = {nullptr, nullptr, nullptr, nullptr, nullptr};
        vm_handle = nullptr;
        return exit_code;
    }

    Result<void*> NovaSharpVM::create_callable_ptr(const std::string& assembly_name,
                                                   const std::string& full_qualified_type,
                                                   const std::string& method) {
        void *delegate;

        int ret = functions.create_delegate(
            vm_handle,
            domain_id,
            assembly_name.c_str(),
            full_qualified_type.c_str(),
            method.c_str(),
            &delegate
            );

        if(ret != 0) {
            return Result<void*>::error(NovaSharpError(ErrorType::ExecutionFailed, ret, "CoreCLR execution failed"));
        } else {
            return Result<void*>::ok(delegate);
        }
    }

    Result<uint32_t> NovaSharpVM::execute(const std::string& assembly_name, const std::vector<std::string>& args) {
        uint32_t exit_code;

        std::vector<const char *> argv;
        argv.reserve(args.size());

        for(const auto &arg : args) {
            argv.emplace_back(arg.c_str());
        }

        int ret = functions.execute_assembly(
            vm_handle,
            domain_id,
            args.size(),
            argv.data(),
            assembly_name.c_str(),
            &exit_code
            );

        if(ret != 0) {
            return Result<uint32_t>::error(NovaSharpError(ErrorType::ExecutionFailed, ret, "CoreCLR execution failed"));
        } else {
            return Result<uint32_t>::ok(exit_code);
        }
    }
} // namespace nova::sharp
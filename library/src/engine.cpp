#include "nova_sharp/engine.hpp"

#include <vector>

#include "nova_sharp/platform.hpp"
#include "nova_sharp/vm.hpp"

#define LOAD_OR_ERROR(target, name)                                                                                                        \
    {                                                                                                                                      \
        auto load_result = lookup_native_symbol(lib, name);                                                                                \
        if(!load_result) {                                                                                                                 \
            return load_result.convert_failed<NovaSharpEngine>();                                                                          \
        }                                                                                                                                  \
        target = reinterpret_cast<typeof(target)>(load_result.value());                                                                    \
    }

namespace nova::sharp {
    Result<NovaSharpEngine> NovaSharpEngine::load(const std::filesystem::path& coreclr_path) {
        auto loaded = load_native_library(coreclr_path);
        if(!loaded) {
            return loaded.convert_failed<NovaSharpEngine>();
        }

        void* lib = loaded.value();

        CoreCLRFunctions functions{};
        LOAD_OR_ERROR(functions.initialize, "coreclr_initialize");
        LOAD_OR_ERROR(functions.shutdown, "coreclr_shutdown");
        LOAD_OR_ERROR(functions.shutdown_2, "coreclr_shutdown_2");
        LOAD_OR_ERROR(functions.create_delegate, "coreclr_create_delegate");
        LOAD_OR_ERROR(functions.execute_assembly, "coreclr_execute_assembly");

        return Result<NovaSharpEngine>::ok(NovaSharpEngine(lib, functions));
    }

    NovaSharpEngine::NovaSharpEngine(void* platform_handle, CoreCLRFunctions functions)
        : platform_handle(platform_handle), functions(functions) {}

    NovaSharpEngine::NovaSharpEngine(NovaSharpEngine&& other) noexcept
        : platform_handle(other.platform_handle), functions(other.functions) {
        other.platform_handle = nullptr;
        other.functions = {nullptr, nullptr, nullptr, nullptr, nullptr};
    }

    NovaSharpEngine::~NovaSharpEngine() {
        if(platform_handle) {
            unload_native_library(platform_handle);
        }
    }

    Result<NovaSharpVM> NovaSharpEngine::create_vm(const std::string& exe_path,
                                                   const std::string& domain_name,
                                                   const std::unordered_map<std::string, std::string>& properties) {
        std::vector<const char*> keys;
        keys.reserve(properties.size());

        std::vector<const char*> values;
        values.reserve(properties.size());

        for(const auto& [key, value] : properties) {
            keys.emplace_back(key.c_str());
            values.emplace_back(value.c_str());
        }

        void* vm_handle;
        uint32_t domain_id;

        int ret = functions.initialize(exe_path.c_str(),
                                       domain_name.c_str(),
                                       properties.size(),
                                       keys.data(),
                                       values.data(),
                                       &vm_handle,
                                       &domain_id);

        if(ret < 0) {
            return Result<NovaSharpVM>::error(NovaSharpError(ErrorType::InitFailed, ret, "CoreCLR init failed"));
        } else {
            return Result<NovaSharpVM>::ok(std::move(NovaSharpVM(vm_handle, domain_id, functions)));
        }
    }

} // namespace nova::sharp
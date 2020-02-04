#include <filesystem>
#include <iostream>

#include <nova_sharp/engine.hpp>
#include <nova_sharp/vm.hpp>

int main(int argc, const char** argv) {
    using namespace nova::sharp;

    auto working_dir = std::filesystem::current_path();
    std::cout << "Running in " << working_dir << std::endl;

    auto engine_result = NovaSharpEngine::load(TEST_HOST_BIN_DIR "/libcoreclr.so");
    if(!engine_result) {
        std::cerr << "Failed to load engine: " << engine_result.error().message << std::endl;
        return 1;
    }

    auto engine = std::move(engine_result.value());
    std::cout << "Engine loaded!" << std::endl;

    // clang-format off
    auto vm_result = engine.create_vm(
        argv[0],
        "nova-sharp",
        {
            {
                "APP_PATHS",
                TEST_HOST_BIN_DIR
            },
            {
                "TRUSTED_PLATFORM_ASSEMBLIES",
                TEST_HOST_BIN_DIR "/System.Private.CoreLib.dll"
            }
        }
    );
    // clang-format on

    if(!vm_result) {
        std::cerr << "Failed to create VM: " << vm_result.error().message << " (0x" << std::hex << vm_result.error().error_code << ")"
                  << std::dec << std::endl;
        return 1;
    }

    auto vm = std::move(vm_result.value());
    std::cout << "VM created!" << std::endl;

    auto version_entrypoint_result = vm.create_callable<char*()>("Test", "NovaSharp.SharpMain", "Version");
    if(!version_entrypoint_result) {
        std::cerr << "Failed to find Test#NovaSharp.SharpMain.Version() method: " << version_entrypoint_result.error().message << std::endl;
        return 1;
    }

    std::cout << "Entrypoint loaded!" << std::endl;
    auto get_novasharp_managed_version = version_entrypoint_result.value();

    std::cout << "NovaSharp managed version: " << get_novasharp_managed_version() << std::endl;

    return 0;
}
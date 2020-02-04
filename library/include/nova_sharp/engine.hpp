#pragma once

#include <filesystem>
#include <unordered_map>

#include "dotnet/coreclrhost.h"
#include "nova_sharp/result.hpp"

namespace nova::sharp {
    class NovaSharpVM;

    struct CoreCLRFunctions {
        coreclr_initialize_ptr initialize;
        coreclr_shutdown_ptr shutdown;
        coreclr_shutdown_2_ptr shutdown_2;

        coreclr_create_delegate_ptr create_delegate;
        coreclr_execute_assembly_ptr execute_assembly;
    };

    class NovaSharpEngine {
    private:
        /*!
         * \brief This is the platform native handle to the CoreCLR library,
         *        a void* on Unix (including OSX) and a HANDLE on Windows
         */
        void* platform_handle;

        /*!
         * \brief Contains the functions used to interact with CoreCLR
         */
        CoreCLRFunctions functions;

        /*!
         * \brief Constructor which is private to allow failing initialization
         *        without throw
         */
        NovaSharpEngine(void* platform_handle, CoreCLRFunctions functions);

    public:
        /*!
         * \brief Tries to load NovaSharp with the specified CoreCLR library
         *
         * \param coreclr_path The path to the libcoreclr.so or coreclr.dll
         * \return A result with the engine loaded or the error
         */
        static Result<NovaSharpEngine> load(const std::filesystem::path& coreclr_path);

        ~NovaSharpEngine();

        NovaSharpEngine(const NovaSharpEngine& other) = delete;
        NovaSharpEngine(NovaSharpEngine&& other) noexcept;

        NovaSharpEngine& operator=(const NovaSharpEngine& other) = delete;
        NovaSharpEngine& operator=(NovaSharpEngine&& other) noexcept = delete;

        /**!
         * \brief Creates a new CoreCLR vm
         *
         * \param exe_path The path of the application creating the VM
         * \param domain_name The .NET root domain name
         * \param properties The properties passed to the CoreCLR vm
         * \return The created VM instance if successful, an error otherwise
         */
        Result<NovaSharpVM> create_vm(const std::string& exe_path,
                                      const std::string& domain_name,
                                      const std::unordered_map<std::string, std::string>& properties);
    };
} // namespace nova::sharp
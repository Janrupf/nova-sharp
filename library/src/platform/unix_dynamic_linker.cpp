#include "nova_sharp/platform.hpp"

extern "C" {
#include <dlfcn.h>
}

namespace nova::sharp {
    std::string get_dl_error() {
        char* error = dlerror();
        return error ? error : "Unknown";
    }

    Result<void*> load_native_library(const std::filesystem::path& path) {
        void* lib = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if(!lib) {
            return Result<void*>::error(NovaSharpError(ErrorType::NativeLibraryLoadFailed, 0, get_dl_error()));
        }

        return Result<void*>::ok(lib);
    }

    Result<void*> lookup_native_symbol(void* library, const std::string& name) {
        void* symbol = dlsym(library, name.c_str());
        if(!symbol) {
            return Result<void*>::error(NovaSharpError(ErrorType::NativeSymbolLookupFailed, 0, get_dl_error()));
        }

        return Result<void*>::ok(symbol);
    }

    void unload_native_library(void* library) {
        dlclose(library);
    }
} // namespace nova::sharp
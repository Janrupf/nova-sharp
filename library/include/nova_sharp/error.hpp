#pragma once

#include <string>

namespace nova::sharp {
    enum ErrorType {
        FsError,
        NativeLibraryLoadFailed,
        NativeSymbolLookupFailed,
        InitFailed,
        ExecutionFailed
    };

    struct NovaSharpError {
        ErrorType type;
        int error_code;
        std::string message;

        NovaSharpError(ErrorType type, int error_code, std::string message);
    };
}
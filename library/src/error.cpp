#include "nova_sharp/error.hpp"

namespace nova::sharp {
    NovaSharpError::NovaSharpError(ErrorType type, int error_code, std::string message)
        : type(type), error_code(error_code), message(std::move(message)) {}
} // namespace nova::sharp

#pragma once

#include <utility>

#ifndef NDEBUG
#include <cassert>

#define assert_is_ok(expected) assert(this->is_ok == expected && "Result is in an incorrect state")
#else
#define assert_is_ok(state)
#endif

#include "nova_sharp/error.hpp"

namespace nova::sharp {
    template <typename TOk>
    class Result {
    private:
        bool is_ok;

        union {
            char uninit{'U'};
            TOk ok_value;
            NovaSharpError error_value;
        };

    public:
        Result(TOk value) : is_ok(true), ok_value(std::move(value)) {}
        Result(NovaSharpError error) : is_ok(false), error_value(std::move(error)) {}

        static Result ok(TOk value) { return Result(std::move(value)); }
        static Result error(NovaSharpError error) { return Result(std::move(error)); }

        ~Result() {
            if(is_ok) {
                ok_value.~TOk();
            } else {
                error_value.~NovaSharpError();
            }
        }

        TOk& value() {
            assert_is_ok(true);
            return ok_value;
        }

        [[nodiscard]] const TOk& value() const {
            assert_is_ok(true);
            return ok_value;
        }

        [[nodiscard]] const NovaSharpError& error() const {
            assert_is_ok(false);
            return error_value;
        }

        explicit operator bool() { return is_ok; }

        template <typename NewTOk>
        Result<NewTOk> convert_failed() {
            assert_is_ok(false);
            return Result<NewTOk>(error_value);
        }
    };
} // namespace nova::sharp
#pragma once

#include <filesystem>
#include <string>

#include "nova_sharp/result.hpp"

namespace nova::sharp {
    /*!
     * \brief Loads a native library and returns a handle to it
     *
     * \param path The path to load the native library from
     * \return A handle if successful, an error otherwise
     *
     * \warning The implementation has to keep track of how many times this function
     *          has been called in combination with unload_native_library. So if this
     *          function succeeded 3 times opening the same library, unload_native_library
     *          has to be called 3 times to actually unload the library. Moreover, opening
     *          the same library multiple times has to return the same handle every time.
     */
    Result<void*> load_native_library(const std::filesystem::path& path);

    /**!
     * \brief Looks for a symbol in a dynamically loaded library
     *
     * \param library The library to search in
     * \param name The name of the symbol to lookup
     * \return A pointer to the symbol if successful, an error otherwise
     */
    Result<void*> lookup_native_symbol(void* library, const std::string& name);

    /*!
     * \brief Unloads a native library
     *
     * \param library The handle of the library to unload
     *
     * \warning See load_native_library for more information
     */
    void unload_native_library(void* library);
} // namespace nova::sharp
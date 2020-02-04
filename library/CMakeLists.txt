######################
# Nova sharp library #
######################

# Source files
set(NOVA_SHARP_LIBRARY_SOURCE
        src/engine.cpp
        src/error.cpp
        src/vm.cpp)

# Header files
set(NOVA_SHARP_INCLUDES
        include/dotnet/coreclrhost.h

        include/nova_sharp/engine.hpp
        include/nova_sharp/error.hpp
        include/nova_sharp/platform.hpp
        include/nova_sharp/result.hpp
        include/nova_sharp/vm.hpp)

# Main include directory
set(NOVA_SHARP_INCLUDE_DIR include)

if(UNIX)
    set(NOVA_SHARP_PLATFORM_SOURCE
            src/platform/unix_dynamic_linker.cpp)

    set(NOVA_SHARP_PLATFORM_LIBS dl)
elseif(WIN32)
    # TODO: WINDOWS
    message(FATAL_ERROR "Windows has not been implemented yet")
else()
    message(FATAL_ERROR "No dynamic loading mechanism available for this platform")
endif()

#########################
# Add nova-sharp target #
#########################
add_library(nova-sharp ${NOVA_SHARP_LIBRARY_SOURCE} ${NOVA_SHARP_INCLUDES} ${NOVA_SHARP_PLATFORM_SOURCE})
target_include_directories(nova-sharp PUBLIC ${NOVA_SHARP_INCLUDE_DIR})
target_link_libraries(nova-sharp PUBLIC ${NOVA_SHARP_PLATFORM_LIBS})
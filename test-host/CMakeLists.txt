########################
# Nova sharp test host #
########################

# Source files
set(NOVA_SHARP_TEST_HOST_SOURCE
        src/main.cpp)

set(NOVA_SHARP_TEST_HOST_BIN_DIR
        bin)

get_filename_component(NOVA_SHARP_TEST_HOST_BIN_DIR "${NOVA_SHARP_TEST_HOST_BIN_DIR}" REALPATH)

#########################
# Add nova-sharp target #
#########################
add_executable(nova-sharp-test-host ${NOVA_SHARP_TEST_HOST_SOURCE})
target_link_libraries(nova-sharp-test-host PUBLIC nova-sharp)
target_compile_definitions(nova-sharp-test-host PUBLIC TEST_HOST_BIN_DIR="${NOVA_SHARP_TEST_HOST_BIN_DIR}")
######################
# Nova Sharp project #
######################
cmake_minimum_required(VERSION 3.16)
project(nova_sharp)

#####################
# Set CMake options #
#####################
set(CMAKE_CXX_STANDARD 17)

############################
# Add NovaSharp components #
############################
add_subdirectory(library)
add_subdirectory(test-host)

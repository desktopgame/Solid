cmake_minimum_required(VERSION 3.25)
project(
    Lib-Math
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
add_library(
    Lib-Math
    INTERFACE
)
target_include_directories(
    Lib-Math
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

enable_testing()
mod_test(Math)
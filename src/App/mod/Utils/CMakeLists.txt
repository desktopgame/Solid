cmake_minimum_required(VERSION 3.25)
project(
    Lib-Utils
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
add_library(
    Lib-Utils
    STATIC
    lib/Utils/String.cpp
)
lib_target_compile_params(Lib-Utils)
lib_target_link_params(Lib-Utils)
target_include_directories(
    Lib-Utils
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

enable_testing()
mod_test(Utils)
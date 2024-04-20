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
    STATIC
    ./lib/Math/Mathf.cpp
)
common_target_compile_params(Lib-Math)
common_target_link_params(Lib-Math)
target_include_directories(
    Lib-Math
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

enable_testing()
mod_test(Math)
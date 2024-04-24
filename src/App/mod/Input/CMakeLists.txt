cmake_minimum_required(VERSION 3.25)
project(
    Lib-Input
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
add_library(
    Lib-Input
    STATIC
    lib/Input/Gamepad.cpp
)
common_target_compile_params(Lib-Input)
common_target_link_params(Lib-Input)
target_include_directories(
    Lib-Input
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
    Lib-Input
    PRIVATE
    "xinput.lib"
)
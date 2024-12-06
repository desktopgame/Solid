cmake_minimum_required(VERSION 3.25)
project(
    Lib-Input
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
set(SOURCES
    ./include/Input.hpp
    ./include/Input/InputSystem.hpp
    ./include/Input/Gamepad.hpp
    ./include/Input/Keyboard.hpp
    ./include/Input/Mouse.hpp
    ./lib/Input/InputSystem.cpp
    ./lib/Input/Gamepad.cpp
    ./lib/Input/Keyboard.cpp
    ./lib/Input/Mouse.cpp
)
source_group(
    TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    ${SOURCES}
)
add_library(
    Lib-Input
    STATIC
    ${SOURCES}
)
lib_target_compile_params(Lib-Input)
lib_target_link_params(Lib-Input)
target_include_directories(
    Lib-Input
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/OS/include"
    "${MODS}/Math/include"
)
target_link_libraries(
    Lib-Input
    PRIVATE
    "xinput.lib"
    Lib-OS
    Lib-Math
)
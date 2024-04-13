cmake_minimum_required(VERSION 3.25)
project(
    Solid
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Library Settings
#
set(Solid-Include
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
set(Solid-Library
    "d3d12.lib"
    "dxgi.lib"
    "d3dcompiler.lib"
)
#
# Build Options
#
set(SOURCES "lib/main.cpp")
add_executable(
    Solid
    ${SOURCES}
)
common_target_compile_params(Solid)
common_target_link_params(Solid)
target_include_directories(
    Solid
    PRIVATE
    ${Solid-Include}
)
target_link_libraries(
    Solid
    PRIVATE
    ${Solid-Library}
)
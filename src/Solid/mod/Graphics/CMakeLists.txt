cmake_minimum_required(VERSION 3.25)
project(
    Solid-Graphics
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
set(SOURCES "lib/main.cpp")
add_library(
    Solid-Graphics
    STATIC
    ./lib/Graphics/Device.cpp
)
common_target_compile_params(Solid-Graphics)
common_target_link_params(Solid-Graphics)
target_include_directories(
    Solid-Graphics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
    Solid-Graphics
    PRIVATE
    "d3d12.lib"
    "dxgi.lib"
    "d3dcompiler.lib"
)
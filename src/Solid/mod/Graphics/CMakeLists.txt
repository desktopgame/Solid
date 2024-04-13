cmake_minimum_required(VERSION 3.25)
project(
    Lib-Graphics
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
set(SOURCES "lib/main.cpp")
add_library(
    Lib-Graphics
    STATIC
    ./lib/Graphics/Device.cpp
    ./lib/Graphics/Engine.cpp
    ./lib/Graphics/Window.cpp
)
common_target_compile_params(Lib-Graphics)
common_target_link_params(Lib-Graphics)
target_include_directories(
    Lib-Graphics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(
    Lib-Graphics
    PRIVATE
    "d3d12.lib"
    "dxgi.lib"
    "d3dcompiler.lib"
)
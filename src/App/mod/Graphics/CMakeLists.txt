cmake_minimum_required(VERSION 3.25)
project(
    Lib-Graphics
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
find_path(
    DirectXTex-Include
    NAMES
    DirectXTex.h
    PATHS
    ${VCPKG_PACKAGES_DIR}/directxtex_x64-windows
    PATH_SUFFIXES
    include
)
find_library(
    DirectXTex-Library
    NAMES
    DirectXTex
    PATHS
    $<$<CONFIG:Debug>: ${VCPKG_PACKAGES_DIR}/directxtex_x64-windows/debug>
    $<$<CONFIG:Release>: ${VCPKG_PACKAGES_DIR}/directxtex_x64-windows>
    PATH_SUFFIXES
    lib
)
add_library(
    Lib-Graphics
    STATIC
    ./lib/Graphics/Device.cpp
    ./lib/Graphics/Engine.cpp
    ./lib/Graphics/Window.cpp
    ./lib/Graphics/Surface.cpp
    ./lib/Graphics/Shader.cpp
    ./lib/Graphics/Buffer.cpp
    ./lib/Graphics/Texture.cpp
    ./lib/Graphics/Screen.cpp
    ./lib/Graphics/VertexData2D.cpp
    ./lib/Graphics/VertexData3D.cpp
    ./lib/Graphics/RenderContext.cpp
    ./lib/Graphics/RenderParameter.cpp
    ./lib/Graphics/Internal/Swapchain.cpp
    ./lib/Graphics/Internal/Constant.cpp
    ./lib/Graphics/Internal/Error.cpp
    ./lib/Graphics/Internal/Pso.cpp
)
common_target_compile_params(Lib-Graphics)
common_target_link_params(Lib-Graphics)
target_include_directories(
    Lib-Graphics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Math/include"
    "${DirectXTex-Include}"
)
target_link_libraries(
    Lib-Graphics
    PRIVATE
    "d3d12.lib"
    "dxgi.lib"
    "d3dcompiler.lib"
    Lib-Math
    "${DirectXTex-Library}"
)
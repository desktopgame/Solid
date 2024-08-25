cmake_minimum_required(VERSION 3.25)
project(
    Lib-Graphics
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
find_package(directxtex CONFIG REQUIRED)
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
    ./lib/Graphics/Swapchain.cpp
    ./lib/Graphics/Internal/Constant.cpp
    ./lib/Graphics/Internal/Error.cpp
    ./lib/Graphics/Internal/Pso.cpp
)
lib_target_compile_params(Lib-Graphics)
lib_target_link_params(Lib-Graphics)
target_include_directories(
    Lib-Graphics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Math/include"
    "${MODS}/Utils/include"
)
target_link_libraries(
    Lib-Graphics
    PRIVATE
    "d3d12.lib"
    "dxgi.lib"
    "d3dcompiler.lib"
    Lib-Math
    Lib-Utils
    Microsoft::DirectXTex
)
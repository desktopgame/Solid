cmake_minimum_required(VERSION 3.25)
project(
    Lib-Graphics
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Embed Metadata
#
add_custom_command(
    OUTPUT ./include/Graphics/Metadata.hpp
    COMMAND powershell -File Embed.ps1 > ./include/Graphics/Metadata.hpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(Embed ALL
    DEPENDS ./include/Graphics/Metadata.hpp
)
#
# Build Options
#
find_package(directxtex CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(imgui CONFIG REQUIRED)
add_library(
    Lib-Graphics
    STATIC
    ./include/Graphics/Metadata.hpp
    ./lib/Graphics/Device.cpp
    ./lib/Graphics/Engine.cpp
    ./lib/Graphics/Surface.cpp
    ./lib/Graphics/Shader.cpp
    ./lib/Graphics/Buffer.cpp
    ./lib/Graphics/Texture.cpp
    ./lib/Graphics/Screen.cpp
    ./lib/Graphics/Camera.cpp
    ./lib/Graphics/Polygon.cpp
    ./lib/Graphics/Renderer.cpp
    ./lib/Graphics/RenderContext.cpp
    ./lib/Graphics/UniformBuffer.cpp
    ./lib/Graphics/UniformPool.cpp
    ./lib/Graphics/VertexTexCoord2D.cpp
    ./lib/Graphics/VertexTexCoord3D.cpp
    ./lib/Graphics/VertexNormal3D.cpp
    ./lib/Graphics/VertexNormalTexCoord3D.cpp
    ./lib/Graphics/Swapchain.cpp
    ./lib/Graphics/Font.cpp
    ./lib/Graphics/FontFactory.cpp
    ./lib/Graphics/FontMap.cpp
    ./lib/Graphics/FontSprite.cpp
    ./lib/Graphics/FontMetrics.cpp
    ./lib/Graphics/GlobalLight.cpp
    ./lib/Graphics/PointLight.cpp
)
lib_target_compile_params(Lib-Graphics)
lib_target_link_params(Lib-Graphics)
target_include_directories(
    Lib-Graphics
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Math/include"
    "${MODS}/OS/include"
    "${MODS}/Utils/include"
    FREETYPE_INCLUDE_DIRS
    imgui::imgui
)
target_link_libraries(
    Lib-Graphics
    PRIVATE
    "d3d12.lib"
    "dxgi.lib"
    "d3dcompiler.lib"
    Lib-Math
    Lib-OS
    Lib-Utils
    Microsoft::DirectXTex
    Freetype::Freetype
    imgui::imgui
)
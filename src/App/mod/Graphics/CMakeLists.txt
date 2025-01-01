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
    COMMAND powershell -File Rehash.ps1
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
set(SOURCES
    ./include/Graphics.hpp
    ./include/Graphics/Metadata.hpp
    ./include/Graphics/Device.hpp
    ./include/Graphics/Engine.hpp
    ./include/Graphics/Surface.hpp
    ./include/Graphics/Shader.hpp
    ./include/Graphics/Texture.hpp
    ./include/Graphics/Screen.hpp
    ./include/Graphics/Camera.hpp
    ./include/Graphics/Polygon.hpp
    ./include/Graphics/Renderer.hpp
    ./include/Graphics/RenderContext.hpp
    ./include/Graphics/UniformBuffer.hpp
    ./include/Graphics/UniformPool.hpp
    ./include/Graphics/VertexTexCoord2D.hpp
    ./include/Graphics/VertexTexCoord3D.hpp
    ./include/Graphics/VertexNormal3D.hpp
    ./include/Graphics/VertexNormalTexCoord3D.hpp
    ./include/Graphics/VertexParticle2D.hpp
    ./include/Graphics/VertexParticle3D.hpp
    ./include/Graphics/Swapchain.hpp
    ./include/Graphics/Font.hpp
    ./include/Graphics/FontFactory.hpp
    ./include/Graphics/FontMap.hpp
    ./include/Graphics/FontSprite.hpp
    ./include/Graphics/FontMetrics.hpp
    ./include/Graphics/GlobalLight.hpp
    ./include/Graphics/PointLight.hpp
    ./include/Graphics/BloomEffect.hpp
    ./include/Graphics/MatrixStack.hpp
    ./include/Graphics/CpuBuffer.hpp
    ./include/Graphics/GpuBuffer.hpp
    ./include/Graphics/DualBuffer.hpp
    ./lib/Graphics/Device.cpp
    ./lib/Graphics/Engine.cpp
    ./lib/Graphics/Surface.cpp
    ./lib/Graphics/Shader.cpp
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
    ./lib/Graphics/VertexParticle2D.cpp
    ./lib/Graphics/VertexParticle3D.cpp
    ./lib/Graphics/Swapchain.cpp
    ./lib/Graphics/Font.cpp
    ./lib/Graphics/FontFactory.cpp
    ./lib/Graphics/FontMap.cpp
    ./lib/Graphics/FontSprite.cpp
    ./lib/Graphics/FontMetrics.cpp
    ./lib/Graphics/GlobalLight.cpp
    ./lib/Graphics/PointLight.cpp
    ./lib/Graphics/BloomEffect.cpp
    ./lib/Graphics/MatrixStack.cpp
    ./lib/Graphics/CpuBuffer.cpp
    ./lib/Graphics/GpuBuffer.cpp
    ./lib/Graphics/DualBuffer.cpp
)
source_group(
    TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    ${SOURCES}
)
add_library(
    Lib-Graphics
    STATIC
    ${SOURCES}
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

enable_testing()
set(TEST_EMBED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/embed")
mod_test(Graphics)
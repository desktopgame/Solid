cmake_minimum_required(VERSION 3.25)
project(
    Lib-Scene
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
add_library(
    Lib-Scene
    STATIC
    lib/Scene/IScene.cpp
    lib/Scene/SceneManager.cpp
)
lib_target_compile_params(Lib-Scene)
lib_target_link_params(Lib-Scene)
target_include_directories(
    Lib-Scene
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Graphics/include"
    "${MODS}/Math/include"
)
target_link_libraries(
    Lib-Scene
    PRIVATE
    Lib-Graphics
    Lib-Math
)
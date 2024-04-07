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
target_compile_definitions(
    Solid
    PRIVATE
    $<$<CONFIG:Debug>:${STD_DEBUG_DEFINE}>
    $<$<CONFIG:Release>:${STD_RELEASE_DEFINE}>
)
target_compile_options(
    Solid
    PRIVATE
    $<$<CONFIG:Debug>:${STD_DEBUG_COMPILE}>
    $<$<CONFIG:Release>:${STD_RELEASE_COMPILE}>
)
target_link_options(
    Solid
    PRIVATE
    $<$<CONFIG:Debug>: ${STD_DEBUG_LINK}>
    $<$<CONFIG:Release>: ${STD_RELEASE_LINK}>
)
target_compile_features(Solid PRIVATE cxx_std_17)
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
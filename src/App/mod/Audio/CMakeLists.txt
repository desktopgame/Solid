cmake_minimum_required(VERSION 3.25)
project(
    Lib-Audio
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
find_package(OpenAL CONFIG REQUIRED)
set(SOURCES
    ./include/Audio.hpp
    ./include/Audio/AudioSource.hpp
    ./include/Audio/AudioClip.hpp
    ./include/Audio/AudioSystem.hpp
    ./lib/Audio/AudioSource.cpp
    ./lib/Audio/AudioClip.cpp
    ./lib/Audio/AudioSystem.cpp
)
source_group(
    TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    ${SOURCES}
)
add_library(
    Lib-Audio
    STATIC
    ${SOURCES}
)
lib_target_compile_params(Lib-Audio)
lib_target_link_params(Lib-Audio)
target_include_directories(
    Lib-Audio
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Math/include"
    OpenAL::OpenAL
)
target_link_libraries(
    Lib-Audio
    PRIVATE
    Lib-Math
    OpenAL::OpenAL
)
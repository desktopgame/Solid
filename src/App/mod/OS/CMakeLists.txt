cmake_minimum_required(VERSION 3.25)
project(
    Lib-OS
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
set(SOURCES
    ./include/OS.hpp
    ./include/OS/Window.hpp
    ./include/OS/Cursor.hpp
    ./lib/OS/Window.cpp
    ./lib/OS/Cursor.cpp
)
source_group(
    TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    ${SOURCES}
)
add_library(
    Lib-OS
    STATIC
    ${SOURCES}
)
lib_target_compile_params(Lib-OS)
lib_target_link_params(Lib-OS)
target_include_directories(
    Lib-OS
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Math/include"
)
target_link_libraries(
    Lib-OS
    PRIVATE
    Lib-Math
)
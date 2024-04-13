cmake_minimum_required(VERSION 3.25)
project(
    Solid
    VERSION 0.0.1
    LANGUAGES CXX
)
add_subdirectory(./mod)
#
# Build Options
#
set(SOURCES "lib/main.cpp")
add_executable(
    Solid
    ${SOURCES}
)
common_target_compile_params(Solid)
common_target_link_params(Solid)
target_include_directories(
    Solid
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/mod/Graphics/include"
)
target_link_libraries(
    Solid
    INTERFACE
    Solid-Graphics
)
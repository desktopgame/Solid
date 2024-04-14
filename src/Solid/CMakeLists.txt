cmake_minimum_required(VERSION 3.25)
project(
    Solid
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Common Variables
#
set(MODS "${CMAKE_CURRENT_SOURCE_DIR}/mod")
#
# Function
#
function(mod_test target)
    add_subdirectory("${MODS}/${target}/test")
    add_test("${target}-Test" "${CMAKE_CURRENT_BINARY_DIR}/test/Lib-${target}-Test.exe")
    add_custom_target("${target}-Test" COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/test/Lib-${target}-Test.exe")
endfunction()
#
# Submodules
#
add_subdirectory(./mod/Math)
add_subdirectory(./mod/Graphics)
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
    "${MODS}/Math/include"
    "${MODS}/Graphics/include"
)
target_link_libraries(
    Solid
    PRIVATE
    Lib-Graphics
    Lib-Math
)
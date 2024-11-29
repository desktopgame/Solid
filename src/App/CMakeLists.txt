cmake_minimum_required(VERSION 3.25)
project(
    App
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
    add_custom_target("${target}-Test" COMMAND ${CMAKE_CTEST_COMMAND} --verbose DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/test/Lib-${target}-Test.exe")
endfunction()
#
# Submodules
#
add_subdirectory(./mod/Json)
add_subdirectory(./mod/Input)
add_subdirectory(./mod/Math)
add_subdirectory(./mod/Graphics)
add_subdirectory(./mod/OS)
add_subdirectory(./mod/Scene)
add_subdirectory(./mod/Utils)
#
# Build Options
#
add_executable(
    App
    lib/main.cpp
    lib/Common/Graphics/Node.cpp
    lib/Common/Util/FpsController.cpp
    lib/Scenes/Debug/DebugScene.cpp
    lib/Scenes/Demo/DemoScene.cpp
    lib/Scenes/Game/GameScene.cpp
)
app_target_compile_params(App)
app_target_link_params(App)
target_include_directories(
    App
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MODS}/Input/include"
    "${MODS}/Graphics/include"
    "${MODS}/Math/include"
    "${MODS}/OS/include"
    "${MODS}/Scene/include"
    "${MODS}/Utils/include"
)
target_link_libraries(
    App
    PRIVATE
    Lib-Json
    Lib-Input
    Lib-Math
    Lib-Graphics
    Lib-OS
    Lib-Scene
    Lib-Utils
)
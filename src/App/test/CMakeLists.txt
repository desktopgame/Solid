cmake_minimum_required(VERSION 3.10)
project(App-Test CXX)

set(LIBS)
foreach(FILE ${SOURCES})
    list(APPEND LIBS "../${FILE}")
endforeach()

add_executable(App-Test ${LIBS})
set_property(
    TARGET
    App-Test
    PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY
    ${REPOSITORY_ROOT}
)
target_compile_definitions(App-Test PRIVATE _TEST NOMINMAX)
target_compile_options(App-Test PRIVATE $<$<BOOL:${MSVC}>: /utf-8>)
target_compile_features(App-Test PRIVATE cxx_std_20)
target_include_directories(
    App-Test
    PRIVATE
    "../include"
    "${MODS}/Input/include"
    "${MODS}/Graphics/include"
    "${MODS}/Math/include"
    "${MODS}/Audio/include"
    "${MODS}/OS/include"
    "${MODS}/Scene/include"
    "${MODS}/Utils/include"
    "${MODS}/UI/include"
)
target_link_libraries(
    App-Test
    PRIVATE
    Lib-Json
    Lib-Input
    Lib-Math
    Lib-Audio
    Lib-Graphics
    Lib-OS
    Lib-Scene
    Lib-Utils
    Lib-UI
)
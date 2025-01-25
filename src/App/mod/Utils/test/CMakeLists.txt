cmake_minimum_required(VERSION 3.10)
project(Lib-Utils-Test CXX)

add_executable(Lib-Utils-Test main.cpp)
target_compile_options(Lib-Utils-Test PRIVATE $<$<BOOL:${MSVC}>: /utf-8>)
target_compile_features(Lib-Utils-Test PRIVATE cxx_std_20)
target_include_directories(
    Lib-Utils-Test
    PRIVATE
    "../include"
    "${MODS}/Math/include"
)
target_link_libraries(
    Lib-Utils-Test
    PRIVATE
    Lib-Utils
)
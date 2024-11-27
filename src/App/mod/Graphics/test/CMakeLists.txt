cmake_minimum_required(VERSION 3.10)
project(Lib-Graphics-Test CXX)

add_executable(Lib-Graphics-Test main.cpp)
target_compile_definitions(Lib-Graphics-Test PRIVATE TEST_EMBED_DIR="${TEST_EMBED_DIR}")
target_compile_options(Lib-Graphics-Test PRIVATE $<$<BOOL:${MSVC}>: /utf-8>)
target_compile_features(Lib-Graphics-Test PRIVATE cxx_std_17)
target_include_directories(
    Lib-Graphics-Test
    PRIVATE
    "../include"
    "${MODS}/Graphics/include"
)
target_link_libraries(
    Lib-Graphics-Test
    PRIVATE
    Lib-Graphics
    Lib-Math
)
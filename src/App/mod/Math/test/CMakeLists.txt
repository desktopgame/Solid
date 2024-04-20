cmake_minimum_required(VERSION 3.10)
project(Lib-Math-Test CXX)

add_executable(Lib-Math-Test main.cpp)
target_compile_features(Lib-Math-Test PRIVATE cxx_std_17)
target_include_directories(
    Lib-Math-Test
    PRIVATE
    "../include"
)
target_link_libraries(
    Lib-Math-Test
    PRIVATE
    Lib-Math
)
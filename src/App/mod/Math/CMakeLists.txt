cmake_minimum_required(VERSION 3.25)
project(
    Lib-Math
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
set(SOURCES
    ./include/Math.hpp
    ./include/Math/Mathf.hpp
    ./include/Math/Matrix.hpp
    ./include/Math/Quaternion.hpp
    ./include/Math/Raycast.hpp
    ./include/Math/Segment.hpp
    ./include/Math/Vector.hpp
    ./include/Math/Geom/AABB.hpp
    ./include/Math/Geom/OBB.hpp
    ./include/Math/Geom/Collision.hpp
)
source_group(
    TREE
    ${CMAKE_CURRENT_SOURCE_DIR}
    FILES
    ${SOURCES}
)
add_library(
    Lib-Math
    INTERFACE
    ${SOURCES}
)
target_include_directories(
    Lib-Math
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

enable_testing()
mod_test(Math)
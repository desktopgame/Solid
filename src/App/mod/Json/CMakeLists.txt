cmake_minimum_required(VERSION 3.25)
project(
    Lib-Json
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
add_library(
    Lib-Json
    INTERFACE
)
find_path(
    PICOJSON_INCLUDE_DIRS
    NAMES "picojson/picojson.h"
    PATHS "${VCPKG_PACKAGES_DIR}/picojson_x64-windows"
    SUFFIX include)
target_include_directories(Lib-Json INTERFACE ${PICOJSON_INCLUDE_DIRS})
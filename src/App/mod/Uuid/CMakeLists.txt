cmake_minimum_required(VERSION 3.25)
project(
    Lib-Uuid
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Build Options
#
add_library(
    Lib-Uuid
    INTERFACE
)
find_path(
    UUID_INCLUDE_DIRS
    NAMES "stduuid/uuid.h"
    PATHS "${VCPKG_PACKAGES_DIR}/stduuid_x64-windows"
    SUFFIX include)
target_include_directories(Lib-Uuid INTERFACE ${UUID_INCLUDE_DIRS})
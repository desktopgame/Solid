cmake_minimum_required(VERSION 3.25)
set(VCPKG_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/App/mod/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_PACKAGES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/App/mod/vcpkg/packages)
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_TOOLCHAIN_FILE}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(
    Solid
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Common Variables
#
set(SOLID_DEBUG_DEFINE
    SOLID_DEBUG _DEBUG
    _LIBCPP_HAS_NO_ASAN # https://stackoverflow.com/questions/38723374/why-does-xcode-define-libcpp-has-no-asan-when-creating-an-address-sanitized-bui
    $<$<CONFIG:Debug>: _ITERATOR_DEBUG_LEVEL=2>
    $<$<CONFIG:Release>: _ITERATOR_DEBUG_LEVEL=0>
)
set(SOLID_RELEASE_DEFINE SOLID_NDEBUG)
set(SOLID_DEBUG_COMPILE
    -O0
    -g
    -Wall
    -Werror
    $<$<NOT:${WIN32}>:-fsanitize=address>
)
set(SOLID_RELEASE_COMPILE
    -O3
)
set(SOLID_DEBUG_LINK
    $<$<NOT:${WIN32}>:-fsanitize=address>
)
set(SOLID_RELEASE_LINK "")
#
# Function
#
function(app_target_compile_params target)
    target_compile_definitions(
        ${target}
        PRIVATE
        $<$<CONFIG:Debug>:${SOLID_DEBUG_DEFINE}>
        $<$<CONFIG:Release>:${SOLID_RELEASE_DEFINE}>
    )
    target_compile_options(
        ${target}
        PRIVATE
        $<$<CONFIG:Debug>:${SOLID_DEBUG_COMPILE}>
        $<$<CONFIG:Release>:${SOLID_RELEASE_COMPILE}>
    )
    target_compile_features(${target} PRIVATE cxx_std_17)
endfunction()

function(app_target_link_params target)
    target_link_options(
        ${target}
        PRIVATE
        $<$<CONFIG:Debug>: ${SOLID_DEBUG_LINK}>
        $<$<CONFIG:Release>: ${SOLID_RELEASE_LINK}>
    )
endfunction()

function(lib_target_compile_params target)
    target_compile_definitions(
        ${target}
        PRIVATE
        $<$<CONFIG:Debug>:${SOLID_DEBUG_DEFINE} SOLID_ENABLE_INTERNAL>
        $<$<CONFIG:Release>:${SOLID_RELEASE_DEFINE} SOLID_ENABLE_INTERNAL>
    )
    target_compile_options(
        ${target}
        PRIVATE
        $<$<CONFIG:Debug>:${SOLID_DEBUG_COMPILE}>
        $<$<CONFIG:Release>:${SOLID_RELEASE_COMPILE}>
    )
    target_compile_features(${target} PRIVATE cxx_std_17)
endfunction()

function(lib_target_link_params target)
    target_link_options(
        ${target}
        PRIVATE
        $<$<CONFIG:Debug>: ${SOLID_DEBUG_LINK}>
        $<$<CONFIG:Release>: ${SOLID_RELEASE_LINK}>
    )
endfunction()
#
# Sub directories
#
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
add_subdirectory(./App)
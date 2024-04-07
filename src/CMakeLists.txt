cmake_minimum_required(VERSION 3.25)
project(
    Solid
    VERSION 0.0.1
    LANGUAGES CXX
)
#
# Common Variables
#
set(STD_DEBUG_DEFINE STD_DEBUG _DEBUG _LIBCPP_HAS_NO_ASAN) # https://stackoverflow.com/questions/38723374/why-does-xcode-define-libcpp-has-no-asan-when-creating-an-address-sanitized-bui
set(STD_RELEASE_DEFINE STD_NDEBUG)
set(STD_DEBUG_COMPILE
    -O0
    -g
    -Wall
    -Werror
    $<$<NOT:${WIN32}>:-fsanitize=address>
)
set(STD_RELEASE_COMPILE
    -O3
)
set(STD_DEBUG_LINK
    $<$<NOT:${WIN32}>:-fsanitize=address>
)
set(STD_RELEASE_LINK "")
#
# Sub directories
#
add_subdirectory(./Solid)
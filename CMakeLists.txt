cmake_minimum_required(VERSION 3.18)
project(cryptopals
    LANGUAGES C
)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(libs)

include_directories(libs)
add_subdirectory(products)

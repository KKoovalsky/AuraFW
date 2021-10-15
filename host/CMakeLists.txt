cmake_minimum_required(VERSION 3.21)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build configuration")

include(../cmake/dependencies.cmake)
ProvideLlvm()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../cmake/clang_host_build_toolchain_file.cmake CACHE PATH
    "Path to the CMake's toolchain file")

project(AuraHost LANGUAGES CXX C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

file(REAL_PATH ${CMAKE_SOURCE_DIR}/../ PROJECT_ROOT_DIR)

include(../cmake/utilities.cmake)

add_subdirectory(../src/common src/common)

enable_testing()
add_subdirectory(../tests/host tests/host)

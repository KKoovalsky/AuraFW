cmake_minimum_required(VERSION 3.21)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build configuration")

file(REAL_PATH ${CMAKE_CURRENT_LIST_DIR}/../ PROJECT_ROOT_DIR)

include(../cmake/dependencies.cmake)
ProvideLlvm()
ProvideArmGnuToolchain()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../cmake/clang_device_build_toolchain_file.cmake CACHE PATH
    "Path to the CMake's toolchain file")

project(AuraDevice LANGUAGES CXX C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

DownloadAndPopulateJunglesCMakeHelpers()
ProvideFreertos()

include(../cmake/static_code_analysis.cmake)
include(../cmake/device_executable.cmake)

add_subdirectory(../src/common src/common)
add_subdirectory(cube)

add_executable(firmware main.cpp)
MakeDeviceExecutable(firmware)
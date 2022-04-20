cmake_minimum_required(VERSION 3.21)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build configuration")

file(REAL_PATH ${CMAKE_CURRENT_LIST_DIR}/../ PROJECT_ROOT_DIR)

include(../cmake/dependencies.cmake)
ProvideLlvm()
ProvideArmGnuToolchain()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/../cmake/clang_with_arm_gnu_libs_device_toolchain.cmake CACHE PATH
    "Path to the CMake's toolchain file")

project(AuraDevice LANGUAGES CXX C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)

DownloadAndPopulateJunglesCMakeHelpers()
ProvideFreertos()
ProvideJunglesOsHelpers()

include(../cmake/static_code_analysis.cmake)
include(../cmake/device_executable.cmake)
include(../cmake/host_tools.cmake)
include(../cmake/device_utilities.cmake)

add_subdirectory(../src/common src/common)
add_subdirectory(cube)
add_subdirectory(serial_logger)

add_executable(firmware main.cpp)
MakeDeviceExecutable(firmware)

enable_testing()
add_subdirectory(../tests/device tests/device)

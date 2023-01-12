function(ProvideCatch2)

    find_package(Catch2 3 REQUIRED)

endfunction()

function(ProvideLlvm)

    include(FetchContent)

    FetchContent_Declare(
        Llvm
        URL https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz
        URL_HASH SHA256=2c2fb857af97f41a5032e9ecadf7f78d3eff389a5cd3c9ec620d24f134ceb3c8
        PREFIX ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}
        SUBBUILD_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/llvm-subbuild
        SOURCE_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/llvm-src
        BINARY_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/llvm-build
    )

    FetchContent_MakeAvailable(Llvm)

    FetchContent_GetProperties(Llvm SOURCE_DIR LLVM_TOOLCHAIN_SOURCE_DIR)

    set(LLVM_TOOLCHAIN_PATH "${LLVM_TOOLCHAIN_SOURCE_DIR}" CACHE PATH "Path to the LLVM toolchain") 

endfunction()

function(ProvideArmGnuToolchain)

    include(FetchContent)

    FetchContent_Declare(
        ArmGnuToolchain
        URL https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
        URL_HASH MD5=2383e4eb4ea23f248d33adc70dc3227e
        PREFIX ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}
        SUBBUILD_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/armgnutoolchain-subbuild
        SOURCE_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/armgnutoolchain-src
        BINARY_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/armgnutoolchain-build
    )

    FetchContent_MakeAvailable(ArmGnuToolchain)

    FetchContent_GetProperties(ArmGnuToolchain SOURCE_DIR ARM_GNU_TOOLCHAIN_SOURCE_DIR)

    set(ARM_GNU_TOOLCHAIN_PATH "${ARM_GNU_TOOLCHAIN_SOURCE_DIR}" CACHE PATH "Path to the ARM GNU toolchain")

endfunction()

function(ProvideFreertos)

    file(GLOB_RECURSE freertos_config_file ${PROJECT_ROOT_FOR_DEPENDENCIES_CMAKE}/src/*FreeRTOSConfig.h)
    get_filename_component(freertos_config_file_dir "${freertos_config_file}" DIRECTORY)

    set(FREERTOS_HEAP "4" CACHE STRING "FreeRTOS heap model number. 1 .. 5. Or absolute path to custom heap source file")
    set(FREERTOS_CONFIG_FILE_DIRECTORY "${freertos_config_file_dir}" CACHE STRING "Absolute path to the directory with FreeRTOSConfig.h")
    set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "FreeRTOS port name")

    include(FetchContent)
    FetchContent_Declare(freertos_kernel_sources
        GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
        GIT_TAG cf6850583c7c7b69726531dd56fa0264159f1750
        SOURCE_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/freertos_kernel_sources-src
    )
    FetchContent_MakeAvailable(freertos_kernel_sources)

endfunction()

function(ProvideUnity)

    include(FetchContent)
    FetchContent_Declare(unity_project
        GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
        GIT_TAG v2.5.2
        SOURCE_DIR ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/unity_project-src
    )
    FetchContent_MakeAvailable(unity_project)

    FetchContent_GetProperties(unity_project SOURCE_DIR unity_source_dir)

    set(UNITY_SOURCE_DIR ${unity_source_dir} PARENT_SCOPE)

endfunction()

function(ProvideJunglesOsHelpers)

    include(FetchContent)
    FetchContent_Declare(JunglesOsHelpers
        GIT_REPOSITORY https://github.com/KKoovalsky/JunglesOsHelpers.git
        GIT_TAG 7598e8e0ee6fcab6346176b4e21846abb20f9485
        SOURCE_DIR    ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/junglesoshelpers-src
    )

    FetchContent_MakeAvailable(JunglesOsHelpers)

endfunction()

function(ProvidePrintfLibrary)

    include(FetchContent)

    set(BUILD_STATIC_LIBRARY ON)
    set(SUPPORT_LONG_LONG OFF)
    set(ALIAS_STANDARD_FUNCTION_NAMES OFF)
    set(DEFAULT_FLOAT_PRECISION "3")

    FetchContent_Declare(printf_library
        GIT_REPOSITORY https://github.com/eyalroz/printf.git
        GIT_TAG v5.0.0
        SOURCE_DIR    ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/printf_library-src
    )
    FetchContent_MakeAvailable(printf_library)

    FetchContent_GetProperties(printf_library SOURCE_DIR printf_source_dir)
    target_include_directories(printf PUBLIC $<BUILD_INTERFACE:${printf_source_dir}>)

endfunction()


set(PROJECT_ROOT_FOR_DEPENDENCIES_CMAKE ${CMAKE_CURRENT_LIST_DIR}/..)
set(DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE ${PROJECT_ROOT_FOR_DEPENDENCIES_CMAKE}/.deps)

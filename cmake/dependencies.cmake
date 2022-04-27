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

function(DownloadAndPopulateJunglesCMakeHelpers)

    include(FetchContent)

    FetchContent_Declare(
        cmake_helpers
        GIT_REPOSITORY https://github.com/KKoovalsky/CMakeHelpers.git
        GIT_TAG e81be067115c349a55715e325ebed98795d55cfa
        SOURCE_DIR    ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/cmake_helpers-src
    )
    FetchContent_MakeAvailable(cmake_helpers)

endfunction()


function(ProvideFreertos)

    JunglesHelpers_DownloadAndPopulateFreeRTOSKernel(freertos V10.4.3 heap_4)
    target_include_directories(freertos PUBLIC ${FREERTOS_SOURCE_DIR}/portable/GCC/ARM_CM4F)
    target_sources(freertos PUBLIC ${FREERTOS_SOURCE_DIR}/portable/GCC/ARM_CM4F/port.c)

    # freertos_config shall be supplied externally. It shall contain at least include path to the FreeRTOSConfig.h.
    target_link_libraries(freertos PUBLIC freertos_config)

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
        GIT_TAG d0644fbb6eacbef5190a1c85f5640a3df60abb9b
        SOURCE_DIR    ${DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE}/junglesoshelpers-src
    )

    FetchContent_MakeAvailable(JunglesOsHelpers)
    target_link_libraries(JunglesOsHelpers INTERFACE freertos)

endfunction()

set(PROJECT_ROOT_FOR_DEPENDENCIES_CMAKE ${CMAKE_CURRENT_LIST_DIR}/..)
set(DEPENDENCIES_DIR_FOR_DEPENDENCIES_CMAKE ${PROJECT_ROOT_FOR_DEPENDENCIES_CMAKE}/.deps)

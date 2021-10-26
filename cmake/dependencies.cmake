function(ProvideCatch2)

    include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        4436a604566f18091489b55af215b8d32afe657a)

    # TODO: PREFIX, subbuild and src (expect build) can be the same for host and the device build. We need to check out
    # what is subbuild and whether it is system dependent. See how is PREFIX, SUBBUILD_DIR, ... used in ProvideLlvm()

    FetchContent_MakeAvailable(Catch2)

endfunction()

function(ProvideLlvm)

    include(FetchContent)

    set(COMMON_DEPENDENCIES_PATH ${CMAKE_SOURCE_DIR}/../build/common_dependencies)

    FetchContent_Declare(
        Llvm
        URL https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz
        URL_HASH SHA256=2c2fb857af97f41a5032e9ecadf7f78d3eff389a5cd3c9ec620d24f134ceb3c8
        PREFIX ${COMMON_DEPENDENCIES_PATH}
        SUBBUILD_DIR ${COMMON_DEPENDENCIES_PATH}/llvm-subbuild
        SOURCE_DIR ${COMMON_DEPENDENCIES_PATH}/llvm-src
        BINARY_DIR ${COMMON_DEPENDENCIES_PATH}/llvm-build
    )

    FetchContent_MakeAvailable(Llvm)

    FetchContent_GetProperties(Llvm SOURCE_DIR LLVM_TOOLCHAIN_SOURCE_DIR)

    set(LLVM_TOOLCHAIN_PATH "${LLVM_TOOLCHAIN_SOURCE_DIR}" CACHE PATH "Path to the LLVM toolchain") 

endfunction()

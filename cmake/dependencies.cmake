function(ProvideCatch2)

    Include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        efd8cc8777406457a34e15313076ac3a77d4e54b)

    FetchContent_MakeAvailable(Catch2)

endfunction()

function(ProvideLlvm)

    Include(FetchContent)

    set(COMMON_DEPENDENCIES_PATH ${CMAKE_BINARY_DIR}/../common_dependencies)

    FetchContent_Declare(
        Llvm
        URL https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.0/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz
        URL_HASH SHA256=a9ff205eb0b73ca7c86afc6432eed1c2d49133bd0d49e47b15be59bbf0dd292e
        PREFIX ${COMMON_DEPENDENCIES_PATH}
        SUBBUILD_DIR ${COMMON_DEPENDENCIES_PATH}/llvm-subbuild
        SOURCE_DIR ${COMMON_DEPENDENCIES_PATH}/llvm-src
        BINARY_DIR ${COMMON_DEPENDENCIES_PATH}/llvm-build
    )

    FetchContent_MakeAvailable(Llvm)

    FetchContent_GetProperties(Llvm SOURCE_DIR LLVM_TOOLCHAIN_SOURCE_DIR)

    set(LLVM_TOOLCHAIN_PATH "${LLVM_TOOLCHAIN_SOURCE_DIR}" CACHE PATH "Path to the LLVM toolchain") 

endfunction()

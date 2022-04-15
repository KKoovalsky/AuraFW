set(CLANG_COMPILER_PATH_PREFIX ${LLVM_TOOLCHAIN_PATH}/bin)

# Without that you will get linking error related to no _exit symbol when CMake tests gcc compile.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSTEM_NAME Generic)

if(DEFINED ENV{CC})
    set(CMAKE_C_COMPILER $ENV{CC})
else()
    set(CMAKE_C_COMPILER ${CLANG_COMPILER_PATH_PREFIX}/clang)
endif()

if(DEFINED ENV{CXX})
    set(CMAKE_CXX_COMPILER $ENV{CXX})
else()
    set(CMAKE_CXX_COMPILER ${CLANG_COMPILER_PATH_PREFIX}/clang++)
endif()

set(CMAKE_ASM_COMPILER ${CLANG_COMPILER_PATH_PREFIX}/clang)
set(CMAKE_LINKER ${CLANG_COMPILER_PATH_PREFIX}/lld)

set(CMAKE_AR ${CLANG_COMPILER_PATH_PREFIX}/llvm-ar)
set(CMAKE_OBJCOPY ${CLANG_COMPILER_PATH_PREFIX}/llvm-objcopy)
set(CMAKE_OBJCOPY_BIN ${CLANG_COMPILER_PATH_PREFIX}/llvm-objcopy)
set(CMAKE_RANLIB ${CLANG_COMPILER_PATH_PREFIX}/llvm-ranlib)
set(CMAKE_STRIP ${CLANG_COMPILER_PATH_PREFIX}/llvm-strip)
set(CMAKE_SIZE_BIN ${CLANG_COMPILER_PATH_PREFIX}/llvm-size)

set(basic_architecture_flags 
    "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")

string(CONCAT basic_flags
    " -Wall -Wextra"
    " ${basic_architecture_flags}"
    " -nodefaultlibs"
    " --sysroot=${ARM_GNU_TOOLCHAIN_PATH}/arm-none-eabi"
    " -fdata-sections -ffunction-sections"
    " -flto"
)

# try_compile() command will reevaluate the toolchain file, but with an on-the-side CMake invocation, thus the 
# CMake CACHE variables will not be preserved. CMake provides a special variable CMAKE_TRY_COMPILE_PLATFORM_VARIABLES,
# which allows to propagate the needed variables from the main build. The command below will be evaluated during
# first inclusion of the toolchain file, so during each and every call to try_compile() it will be already set.
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES ARM_GNU_TOOLCHAIN_PATH)

include(${CMAKE_CURRENT_LIST_DIR}/arm_gnu_toolchain_utils.cmake)
ArmGnu_GetCxxSystemIncludeFlags(${basic_architecture_flags} cxx_system_includes)
ArmGnu_GetStandardLibrariesDirectory(${basic_architecture_flags} standard_libraries_dir)
ArmGnu_GetCompilerRuntimeLibrariesDirectory(${basic_architecture_flags} compiler_runtime_libraries_dir)

string(CONCAT exe_linker_flags
    " -Wl,--sysroot=${ARM_GNU_TOOLCHAIN_PATH}/arm-none-eabi"
    " -Wl,--gc-sections"
    " -Wl,-Map=output.map"
    " -Wl,-nostdlib"
    " -Wl,--target2=rel"
    " -L${standard_libraries_dir}"
    " -L${compiler_runtime_libraries_dir}"
    # This order of linking is mandatory: https://gcc.gnu.org/onlinedocs/gccint/Initialization.html
    " ${compiler_runtime_libraries_dir}/crti.o"
    " ${compiler_runtime_libraries_dir}/crtbegin.o"
    " ${standard_libraries_dir}/crt0.o"
    " -lc -lm -lg -lnosys -lstdc++ -lgcc"
    " ${compiler_runtime_libraries_dir}/crtend.o"
    " ${compiler_runtime_libraries_dir}/crtn.o"
    " -flto"
)

set(CMAKE_C_FLAGS_INIT "${basic_flags}")
set(CMAKE_ASM_FLAGS_INIT  "${basic_architecture_flags}")
set(CMAKE_CXX_FLAGS_INIT "${basic_flags} ${cxx_system_includes}")

set(CMAKE_EXE_LINKER_FLAGS_INIT "${exe_linker_flags}")

set(CMAKE_C_COMPILER_TARGET arm-none-eabi)
set(CMAKE_CXX_COMPILER_TARGET arm-none-eabi)
set(CMAKE_ASM_COMPILER_TARGET arm-none-eabi)

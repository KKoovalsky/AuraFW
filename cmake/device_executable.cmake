##########################################################################################
# Function definitions
##########################################################################################

#! AddDeviceExecutable Wrapper for add_executable() to create proper, flashable device executable. 
# 
# This function uses the same API as add_executable(), so the first argument is the executable name and the
# rest are the source files forwareded to add_executable().
function(AddDeviceExecutable target_name)

    add_executable(${target_name} ${ARGN})
    MakeDeviceExecutable(${target_name})

endfunction()

function(MakeDeviceExecutable target_name)

    LinkDeviceSpecificDetails(${target_name})
    GenerateHexAfterBuild(${target_name})
    PrintBinarySizeAfterBuild(${target_name})
    AddFlashTargets(${target_name})

endfunction()

function(LinkDeviceSpecificDetails target_name)

    set(cube_generated_files_dir "${AURA_PROJECT_ROOT_DIR_FOR_DEVICE_EXECUTABLE_CMAKE}/src/device/cube/Aura")

    set(linker_script "${cube_generated_files_dir}/STM32L432KCUx_FLASH.ld")
    target_link_options(${target_name} PRIVATE -T${linker_script} -Wl,-Map=${target_name}.map)

    target_link_libraries(${target_name} PRIVATE device_specific cube)

    target_sources(${target_name} PRIVATE $<TARGET_OBJECTS:device_specific>)

endfunction()

function(GenerateHexAfterBuild target_name)

    set(target_hex_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.hex)
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target_name}> ${target_hex_file}
        BYPRODUCTS ${target_hex_file}
        COMMENT "Generating executable HEX file for ${target_name}"
    )

endfunction()

function(PrintBinarySizeAfterBuild target_name)

    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${target_name}>
        COMMENT "Section sizes of the executable binary"
    )

endfunction()

function(AddFlashTargets target_name)

    find_program(OPENOCD_PROGRAM NAMES openocd REQUIRED)

    add_custom_target(${target_name}-flash
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg
            -c "program $<TARGET_FILE:${target_name}> ; reset ; exit"
        VERBATIM
    )

    add_custom_target(${target_name}-flash_no_run
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg
            -c "program $<TARGET_FILE:${target_name}> ; reset halt ; exit"
        VERBATIM
    )

endfunction()

function(CreateDeviceSpecificLibraries)

    set(cube_generated_files_dir "${AURA_PROJECT_ROOT_DIR_FOR_DEVICE_EXECUTABLE_CMAKE}/src/device/cube/Aura")
    set(startup_file "${cube_generated_files_dir}/startup_stm32l432xx.s")

    set(device_dir "${AURA_PROJECT_ROOT_DIR_FOR_DEVICE_EXECUTABLE_CMAKE}/src/device")
    set(custom_alloc_file ${device_dir}/custom_alloc.c)
    set(custom_terminate_file "${device_dir}/custom_terminate.cpp")

    add_library(device_specific OBJECT ${startup_file} ${custom_alloc_file} ${custom_terminate_file})

endfunction()

##########################################################################################
# Main script
##########################################################################################

set(AURA_PROJECT_ROOT_DIR_FOR_DEVICE_EXECUTABLE_CMAKE ${CMAKE_CURRENT_LIST_DIR}/..)

CreateDeviceSpecificLibraries()

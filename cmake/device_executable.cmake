##########################################################################################
# Function definitions
##########################################################################################

function(MakeDeviceExecutable target_name)

    LinkDeviceSpecificDetails(${target_name})
    GenerateHexAfterBuild(${target_name})
    PrintBinarySizeAfterBuild(${target_name})
    AddFlashTarget(${target_name})

endfunction()

function(LinkDeviceSpecificDetails target_name)
set(cube_generated_files_dir "${PROJECT_ROOT_DIR}/device/cube/Aura")

    set(linker_script "${cube_generated_files_dir}/STM32L432KCUx_FLASH.ld")
    target_link_options(${target_name} PRIVATE -T${linker_script})

    target_link_libraries(${target_name} PRIVATE device_specific)
    target_link_libraries(${target_name} PRIVATE cube)

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
        COMMAND ${CMAKE_SIZE_BIN} $<TARGET_FILE:${target_name}>
        COMMENT "Section sizes of the executable binary"
    )

endfunction()

function(AddFlashTarget target_name)

    find_program(OPENOCD_PROGRAM NAMES openocd REQUIRED)
    add_custom_target(${target_name}-flash
        COMMAND ${OPENOCD_PROGRAM} -f board/stm32l4discovery.cfg -c "program $<TARGET_FILE:${target_name}> reset exit"
    )

endfunction()

function(CreateDeviceSpecificLibraries)

    set(cube_generated_files_dir "${PROJECT_ROOT_DIR}/device/cube/Aura")
    set(startup_file "${cube_generated_files_dir}/startup_stm32l432xx.s")
    set(clang_init_file "${PROJECT_ROOT_DIR}/device/clang_init.c")

    add_library(device_specific STATIC ${startup_file} ${clang_init_file})

endfunction()

##########################################################################################
# Main script
##########################################################################################
CreateDeviceSpecificLibraries()
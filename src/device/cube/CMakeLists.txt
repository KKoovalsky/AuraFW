include(cube_sources.cmake)
set(cmsis_os_dir Aura/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2)

add_library(cube STATIC ${cube_sources} ${cmsis_os_dir}/cmsis_os2.c)

target_compile_definitions(cube PUBLIC USE_HAL_DRIVER STM32L432xx)
target_include_directories(cube PUBLIC
    Aura/Core/Inc
    Aura/Drivers/STM32L4xx_HAL_Driver/Inc
    Aura/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    Aura/Drivers/CMSIS/Include
    ${cmsis_os_dir}
)
target_link_libraries(cube PRIVATE freertos_kernel)

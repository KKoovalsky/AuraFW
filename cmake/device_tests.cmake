function(CreateDeviceTest target_name)

    MakeDeviceExecutable(${target_name})
    target_link_libraries(${target_name} PRIVATE device_test_setup unity::framework)

endfunction()
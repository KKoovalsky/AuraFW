function(CreateDeviceTest target_name)

    MakeDeviceExecutable(${target_name})
    target_link_libraries(${target_name} PRIVATE device_test_setup unity::framework)

    GenerateTestRunnerOnTestExecutableRebuild(${target_name})

endfunction()

function(GenerateTestRunnerOnTestExecutableRebuild target_name)

    EnsureRubyIsInstalled(ruby_program)
    EnsurePython3IsInstalled(python3_program)

    set(test_runner_generator ${UNITY_SOURCE_DIR}/auto/generate_test_runner.rb)

    set(test_runner_filename ${target_name}_runner.cpp)
    set(test_runner_filepath ${CMAKE_CURRENT_BINARY_DIR}/${test_runner_filename})

    # Use find_file() to get the full path to the source file with the tests.
    # The source file shall be put under the same directory under which the current CMakeLists.txt is put.
    set(test_filename ${target_name}.cpp)
    find_file(test_file ${test_filename}
        PATHS ${CMAKE_CURRENT_SOURCE_DIR}
        REQUIRED
        NO_DEFAULT_PATH
        NO_CACHE)

    set(main_function_renamer ${PROJECT_ROOT_DIR}/scripts/rename_main_to_test_main.py)

    add_custom_command(OUTPUT ${test_runner_filename}
        COMMAND ${ruby_program} ${test_runner_generator} ${test_file} ${test_runner_filepath}
        # Rename main() in the generated file to test_main() to avoid clash with main() from 'cube' directory.
        COMMAND ${python3_program} ${main_function_renamer} ${test_runner_filepath}
        DEPENDS ${test_file}
    )

    set(test_runner ${target_name}_runner)
    # CMake will figure out that the file is an OUTPUT from custom command and create proper dependencies for that.
    add_library(${test_runner} STATIC ${test_runner_filename})
    target_link_libraries(${test_runner} PRIVATE unity::framework)

    target_link_libraries(${target_name} PRIVATE ${test_runner})

endfunction()

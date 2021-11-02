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

    set(post_generation_test_runner_fixer ${PROJECT_ROOT_DIR}/scripts/fix_test_runner_after_generation.py)

    add_custom_command(OUTPUT ${test_runner_filename}
        # Test runner generator will copy all the "include" lines to the generated file. This is unwanted. We use
        # option to put all that includes to a separate header file and then remove the include line from the
        # generated file.
        COMMAND ${ruby_program} ${test_runner_generator}
            --header_file="dummy_header.hpp"
            --main_name="test_main"
            ${test_file} ${test_runner_filepath}
        # The fixer renames main() function to test_main() and removes the inclusion to "dummy_header.hpp"
        COMMAND ${python3_program} ${post_generation_test_runner_fixer} ${test_runner_filepath}
        DEPENDS ${test_file}
    )

    set(test_runner ${target_name}_runner)
    # CMake will figure out that the file is an OUTPUT from custom command and create proper dependencies for that.
    add_library(${test_runner} STATIC ${test_runner_filename})
    target_link_libraries(${test_runner} PRIVATE unity::framework)

    target_link_libraries(${target_name} PRIVATE ${test_runner})

endfunction()

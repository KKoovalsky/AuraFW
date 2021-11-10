#! PromoteToDeviceTestExecutable: Promotes the executable target to be run as device test.
#
# This will enchant 'target_name' to be runnable on the device, by linking proper flags and libraries to it, and
# create <target_name>-flash target that can be used to flash the target to the device.
#
# Moreover, it will promote it, not only to a device executable, but also, a test executable, by linking test
# libraries and generating a runner. The unit testing framework is ThrowTheSwitch/Unity.
#
# This function expects that the target specified as the first argument has source file called <target_name>.cpp,
# which defines tests, starting with "test_*", that are used to generate an Unity runner.
#
# Optional argument NO_UNITY_OUTPUT can be specified to disable forwarding Unity output to serial.
#
# \arg:target_name              Executable target name, which contains the tests.
#
function(PromoteToDeviceTestExecutable target_name)

    set(options NO_UNITY_OUTPUT)
    set(one_value_args )
    set(multi_value_args)
    cmake_parse_arguments(DEVICE_TEST "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN} )

    MakeDeviceExecutable(${target_name})
    target_link_libraries(${target_name} PRIVATE device_test_setup unity::framework)

    GenerateTestRunnerOnTestExecutableRebuild(${target_name} ${DEVICE_TEST_NO_UNITY_OUTPUT})

endfunction()

function(GenerateTestRunnerOnTestExecutableRebuild target_name is_unity_output_disabled)

    EnsureRubyIsInstalled(ruby_program)
    EnsurePython3IsInstalled(python3_program)

    set(test_runner_generator ${UNITY_SOURCE_DIR}/auto/generate_test_runner.rb)

    set(test_runner_filename ${target_name}_runner.cpp)
    set(test_runner_filepath ${CMAKE_CURRENT_BINARY_DIR}/${test_runner_filename})

    # Use find_file() to get the full path to the source file with the tests.
    # The source file shall be put under the same directory under which the current CMakeLists.txt is put.
    set(test_filename ${target_name}.cpp)
    find_file(test_file ${test_filename} PATHS ${CMAKE_CURRENT_SOURCE_DIR} REQUIRED NO_DEFAULT_PATH NO_CACHE)

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

    if(is_unity_output_disabled)
        target_link_libraries(${test_runner} PRIVATE unity_interface_no_output)
    else()
        target_link_libraries(${test_runner} PRIVATE unity_interface)
    endif()

    target_link_libraries(${target_name} PRIVATE ${test_runner})

endfunction()

function(AddSanitizerForSerialPortDefined)

    set(AURA_SERIAL_PORT "" CACHE STRING
        "Serial port on which logs are collected from Aura (e.g. /dev/ttyUSB1 or COM3)")

    set(sanitizer ${PROJECT_ROOT_DIR}/cmake/sanitize_variable_not_empty.cmake)

    string(CONCAT error_message
        "CMake CACHE variable AURA_SERIAL_PORT undefined. "
        "Define it to indicate the port on which the logs from Aura are collected "
        "e.g. '/dev/ttyUSB1' for Linux or 'COM3' for Windows"
    )

    add_test(NAME sanitize_serial_port_defined
             COMMAND ${CMAKE_COMMAND}
                -DNAME=AURA_SERIAL_PORT
                -DVALUE=${AURA_SERIAL_PORT}
                -DERROR_MESSAGE=${error_message}
                -P ${sanitizer})

    set_tests_properties(sanitize_serial_port_defined PROPERTIES FIXTURES_SETUP SerialPort)

endfunction()

##########################################################################################
# Main script
##########################################################################################

AddSanitizerForSerialPortDefined()
function(CreateLoggableStaticAssertionTest header_file_with_logger_definition test_name_prefix output_test_name)

    # Derive name from the header file name. Make sure that the header file name explains the contents verbosely.
    cmake_path(GET header_file_with_logger_definition STEM name)

    # Generate a test file with the Logger class supplied through the header file.
    set(FILE_WITH_LOGGABLE_CLASS_DEFINITION ${header_file_with_logger_definition})
    set(output_file_name test_loggable_${name}.cpp)
    configure_file(test_loggable.cpp.in ${output_file_name})

    set(target_name ${test_name_prefix}_${name}_exec)
    set(test_name ${test_name_prefix}_${name})

    add_executable(${target_name} ${output_file_name})

    # Include directory where the supplied header file is located.
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

    # Include directory where "loggable.hpp" is located.
    target_link_libraries(${target_name} PRIVATE aura_common)

    # Don't build with 'all' target; build only on demand.
    set_target_properties(${target_name} PROPERTIES
        EXCLUDE_FROM_ALL TRUE
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    add_test(NAME ${test_name} COMMAND
        ${CMAKE_COMMAND} --build . --target ${target_name})

    set_tests_properties(${test_name} PROPERTIES LABELS Loggable)

    set(${output_test_name} ${test_name} PARENT_SCOPE)

endfunction()

function(CreateLoggablePositiveStaticAssertionTest header_file_with_logger_definition)

    CreateLoggableStaticAssertionTest(${header_file_with_logger_definition} "loggable_test_positive" test_name_unused)

endfunction()

function(CreateLoggableNegativeStaticAssertionTest header_file_with_logger_definition)

    CreateLoggableStaticAssertionTest(${header_file_with_logger_definition} "loggable_test_negative" test_name)

    # Negative test pass when compilation fails
    set_tests_properties(${test_name} PROPERTIES WILL_FAIL True)

endfunction()

CreateLoggablePositiveStaticAssertionTest(logger_with_all_stream_operator_overloads_defined.h)
CreateLoggablePositiveStaticAssertionTest(logger_with_stream_operator_overloaded_as_template.h)
CreateLoggableNegativeStaticAssertionTest(logger_with_stream_operator_for_float_and_string_overloaded.hpp)
CreateLoggableNegativeStaticAssertionTest(logger_with_no_stream_operator_defined_for_float.hpp)
CreateLoggableNegativeStaticAssertionTest(logger_with_no_stream_operator_defined_for_string.hpp)
CreateLoggableNegativeStaticAssertionTest(logger_with_no_stream_operator_defined_for_unsigned.hpp)
CreateLoggableNegativeStaticAssertionTest(logger_with_no_stream_operator_defined_for_int.hpp)
CreateLoggableNegativeStaticAssertionTest(logger_with_no_stream_operator_defined_for_unsigned_char.hpp)
CreateLoggableNegativeStaticAssertionTest(logger_with_no_stream_operator_defined_for_char.hpp)
CreateLoggableNegativeStaticAssertionTest(logger_with_no_log_method.hpp)
CreateLoggableNegativeStaticAssertionTest(empty_logger.hpp)
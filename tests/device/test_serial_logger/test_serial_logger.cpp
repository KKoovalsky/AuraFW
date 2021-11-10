/**
 * @file    test_serial_logger.cpp
 * @brief   Tests the SerialLogger
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "unity.h"

#include "serial_logger.hpp"

// =====================================================================================================================
// Test cases declaration
// =====================================================================================================================
void test_implements_loggable();

void test_multiple_instances_of_logger_can_be_created_simultaneously();
void test_multiple_instances_of_logger_can_be_created_non_simultaneously();
void test_cannot_use_output_operator_without_calling_log_first();
void test_float_is_printed_with_precision_to_thousandths();
void test_unsigned_char_is_printed_as_hex_char();
void test_unsigned_int_is_printed();
void test_negative_int_is_printed();
void test_positive_int_is_printed();
void test_single_char_is_printed();
void test_warning_log_is_printed();
void test_error_log_is_printed();

// =====================================================================================================================
// Test cases definition
// =====================================================================================================================
void test_implements_loggable()
{
    static_assert(Loggable<SerialLogger>);
}

void test_multiple_instances_of_logger_can_be_created_simultaneously()
{
}

void test_multiple_instances_of_logger_can_be_created_non_simultaneously()
{
}

void test_cannot_use_output_operator_without_calling_log_first()
{
}

void test_float_is_printed_with_precision_to_thousandths()
{
}

void test_unsigned_char_is_printed_as_hex_char()
{
}

void test_unsigned_int_is_printed()
{
}

void test_negative_int_is_printed()
{
}

void test_positive_int_is_printed()
{
}

void test_single_char_is_printed()
{
}

void test_warning_log_is_printed()
{
}

void test_error_log_is_printed()
{
}

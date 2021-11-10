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
void test_simple_log_is_printed();
void test_implements_loggable();
void test_multiple_logs_are_printed();
void test_enormous_number_of_logs_doesnt_clog_the_logger();
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
void test_simple_log_is_printed()
{
    bool is_success{false};
    try
    {
        SerialLogger{}.log(LogLevel::info) << "some simple log";
        is_success = true;
    } catch (const std::exception&)
    {
    }
    TEST_ASSERT(is_success);
}

void test_implements_loggable()
{
    static_assert(Loggable<SerialLogger>);
}

void test_multiple_logs_are_printed()
{
    bool is_success{false};
    try
    {
        SerialLogger serial_logger{};
        serial_logger.log(LogLevel::info) << "some log1";
        serial_logger.log(LogLevel::info) << "some log2";
        serial_logger.log(LogLevel::info) << "some log3";
        serial_logger.log(LogLevel::info) << "some log4";
        serial_logger.log(LogLevel::info) << "some log5";
        serial_logger.log(LogLevel::info) << "some log6";
        is_success = true;
    } catch (const std::exception&)
    {
    }
    TEST_ASSERT(is_success);
}

void test_enormous_number_of_logs_doesnt_clog_the_logger()
{
    // 1000 logs then delay and then one more log
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

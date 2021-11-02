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
}

void test_multiple_logs_are_printed()
{
}

void test_enormous_number_of_logs_doesnt_clog_the_logger()
{
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

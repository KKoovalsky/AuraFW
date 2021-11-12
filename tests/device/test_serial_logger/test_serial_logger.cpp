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
void test_serial_logger_works_with_unity();

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

void test_serial_logger_works_with_unity()
{
}

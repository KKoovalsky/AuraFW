/**
 * @file    custom_terminate.cpp
 * @brief   Defines custom terminate function to spare FLASH size.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <exception>

[[noreturn]] void terminate() noexcept
{
    while (true)
        ;
}

namespace __cxxabiv1
{
std::terminate_handler __terminate_handler = terminate;
}


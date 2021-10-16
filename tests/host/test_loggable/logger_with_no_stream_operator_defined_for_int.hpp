/**
 * @file        logger_with_no_stream_operator_defined_for_int.hpp
 * @brief       Defines a Logger without overload of operator<< for int.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_INT_HPP
#define LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_INT_HPP

#include "loggable.hpp"

#include <string>

struct Logger
{
    Logger& log(LogLevel)
    {
        return *this;
    }

    Logger& operator<<(std::string)
    {
        return *this;
    }

    Logger& operator<<(float)
    {
        return *this;
    }

    Logger& operator<<(unsigned)
    {
        return *this;
    }

    Logger& operator<<(unsigned char)
    {
        return *this;
    }

    Logger& operator<<(char)
    {
        return *this;
    }
};

#endif /* LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_INT_HPP */

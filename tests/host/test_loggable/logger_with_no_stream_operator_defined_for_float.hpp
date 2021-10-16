/**
 * @file        logger_with_no_stream_operator_defined_for_float.hpp
 * @brief       Defines a logger with no operator<< function defined for float parameter.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_FLOAT_HPP
#define LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_FLOAT_HPP

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

    Logger& operator<<(unsigned)
    {
        return *this;
    }

    Logger& operator<<(int)
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

#endif /* LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_FLOAT_HPP */

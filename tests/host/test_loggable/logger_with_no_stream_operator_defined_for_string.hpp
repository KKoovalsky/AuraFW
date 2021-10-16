/**
 * @file        logger_with_no_stream_operator_defined_for_string.hpp
 * @brief       Defines a Logger with no operator<< overload for std::string
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_STRING_HPP
#define LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_STRING_HPP

#include "loggable.hpp"

struct Logger
{
    Logger& log(LogLevel)
    {
        return *this;
    }

    Logger& operator<<(unsigned)
    {
        return *this;
    }

    Logger& operator<<(float)
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

#endif /* LOGGER_WITH_NO_STREAM_OPERATOR_DEFINED_FOR_STRING_HPP */

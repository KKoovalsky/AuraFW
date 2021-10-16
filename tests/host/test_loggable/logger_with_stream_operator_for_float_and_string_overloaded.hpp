/**
 * @file        logger_with_stream_operator_for_float_and_string_overloaded.hpp
 * @brief       Defines a Logger with operator<< overloads only for float and std::string.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGER_WITH_STREAM_OPERATOR_FOR_FLOAT_AND_STRING_OVERLOADED_HPP
#define LOGGER_WITH_STREAM_OPERATOR_FOR_FLOAT_AND_STRING_OVERLOADED_HPP

#include <string>

#include "loggable.hpp"

struct Logger
{
    Logger& log(LogLevel)
    {
        return *this;
    }

    Logger& operator<<(float)
    {
        return *this;
    }

    Logger& operator<<(std::string)
    {
        return *this;
    }
};

#endif /* LOGGER_WITH_STREAM_OPERATOR_FOR_FLOAT_AND_STRING_OVERLOADED_HPP */

/**
 * @file        logger_with_all_stream_operator_overloads_defined.h
 * @brief       Defines a logger with all operator<< overloads defined that satisfy Loggable.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGER_WITH_ALL_STREAM_OPERATOR_OVERLOADS_DEFINED_HPP
#define LOGGER_WITH_ALL_STREAM_OPERATOR_OVERLOADS_DEFINED_HPP

#include "loggable.hpp"

#include <string>

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

#endif /* LOGGER_WITH_ALL_STREAM_OPERATOR_OVERLOADS_DEFINED_HPP */
/**
 * @file        logger_with_stream_operator_overloaded_as_template.h
 * @brief       Defines a logger with templated operator<< overload.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGER_WITH_STREAM_OPERATOR_OVERLOADED_AS_TEMPLATE_HPP
#define LOGGER_WITH_STREAM_OPERATOR_OVERLOADED_AS_TEMPLATE_HPP

#include "loggable.hpp"

struct Logger
{
    Logger& log(LogLevel)
    {
        return *this;
    }

    template<typename T>
    Logger& operator<<(T&&)
    {
        return *this;
    }
};

#endif /* LOGGER_WITH_STREAM_OPERATOR_OVERLOADED_AS_TEMPLATE_HPP */
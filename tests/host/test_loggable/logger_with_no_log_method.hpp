/**
 * @file        logger_with_no_log_method.hpp
 * @brief       Defines a Logger with no log() member method.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGER_WITH_NO_LOG_METHOD_HPP
#define LOGGER_WITH_NO_LOG_METHOD_HPP

#include "loggable.hpp"

#include <string>

struct Logger
{
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

    Logger& operator<<(int)
    {
        return *this;
    }

    Logger& operator<<(char)
    {
        return *this;
    }

    Logger& operator<<(unsigned char)
    {
        return *this;
    }
};

#endif /* LOGGER_WITH_NO_LOG_METHOD_HPP */

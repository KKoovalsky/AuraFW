/**
 * @file        loggable.hpp
 * @brief       Defines Logger concept - compile time interface for a Logger.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef LOGGABLE_HPP
#define LOGGABLE_HPP

#include <concepts>
#include <string>

enum class LogLevel
{
    info,
    warning,
    error
};

template<typename T>
concept WeaklyLoggable = requires(T v)
{
    {
        v.log(LogLevel{})
        } -> std::convertible_to<T&>;
};

template<WeaklyLoggable T, class U>
requires(!std::convertible_to<U, std::string>) auto operator<<(T, U) = delete;

template<typename T>
concept Loggable = WeaklyLoggable<T> && requires(T v)
{
    {
        v << "log message" << static_cast<unsigned>(0) << static_cast<int>(0) << static_cast<float>(0.0)
          << static_cast<unsigned char>(0) << static_cast<char>('0')
        } -> std::convertible_to<T&>;
};

#endif /* LOGGABLE_HPP */
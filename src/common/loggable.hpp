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
concept HasLogMethodReturningReferenceToSelf = requires(T v)
{
    {
        v.log(LogLevel{})
        } -> std::convertible_to<T&>;
};

//! Disallows implicit cast of basic types, allowing only string-literals and string-like to std::string implicit cast.
template<HasLogMethodReturningReferenceToSelf T, class U>
requires(!std::convertible_to<U, std::string>) auto operator<<(T, U) = delete;

template<typename T>
concept HasOutputStreamOperatorOverloadsForBasicTypes =
    requires(T v, unsigned unsigned_, int int_, float float_, unsigned char unsigned_char_, char char_)
{
    {
        v << "string literal" //
          << unsigned_        //
          << int_             //
          << float_           //
          << unsigned_char_   //
          << char_            //
        } -> std::convertible_to<T&>;
};

template<typename T>
concept Loggable = HasLogMethodReturningReferenceToSelf<T> && HasOutputStreamOperatorOverloadsForBasicTypes<T>;

#endif /* LOGGABLE_HPP */
/**
 * @file        measurer.hpp
 * @brief       Measurer interface.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef MEASURER_HPP
#define MEASURER_HPP

#include <exception>

template<typename Measurement>
struct Measurer
{
    virtual Measurement measure() = 0;
    virtual ~Measurer() = default;

    struct Error : std::exception
    {
    };
};

#endif /* MEASURER_HPP */

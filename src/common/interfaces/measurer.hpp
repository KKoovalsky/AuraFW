/**
 * @file        measurer.hpp
 * @brief       Measurer interface.
 */
#ifndef MEASURER_HPP
#define MEASURER_HPP

template<typename Measurement>
struct Measurer
{
    virtual Measurement measure() = 0;
    virtual ~Measurer() = default;
};

#endif /* MEASURER_HPP */

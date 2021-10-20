/**
 * @file        publisher.hpp
 * @brief       Publisher interface.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <exception>
#include <vector>

template<typename Measurement>
struct Publisher
{
    using MultipleMeasurements = std::vector<Measurement>;

    virtual void publish(MultipleMeasurements) = 0;
    virtual ~Publisher() = default;

    struct Error : std::exception
    {
    };
};

#endif /* PUBLISHER_HPP */

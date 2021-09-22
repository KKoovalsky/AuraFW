/**
 * @file        publisher.hpp
 * @brief       Publisher interface.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef PUBLISHER_HPP
#define PUBLISHER_HPP

#include <vector>

template<typename Measurement>
struct Publisher
{
    using MultipleMeasurements = std::vector<Measurement>;

    virtual void publish(MultipleMeasurements) = 0;
    virtual ~Publisher() = default;
};

#endif /* PUBLISHER_HPP */

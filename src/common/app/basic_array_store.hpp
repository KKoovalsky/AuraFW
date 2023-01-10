/**
 * @file        basic_array_store.hpp
 * @brief       A Store implementation which buffers the measurements in an std::array.
 */
#ifndef BASIC_ARRAY_STORE_HPP
#define BASIC_ARRAY_STORE_HPP

#include <array>
#include <stdexcept>

#include "interfaces/store.hpp"

template<typename Measurement, unsigned MaxMeasurements>
struct BasicArrayStore : Store<Measurement, std::array<Measurement, MaxMeasurements>>
{
    using Package = std::array<Measurement, MaxMeasurements>;

    void store(Measurement m) override
    {
        if (idx == package.size())
            throw std::runtime_error{"Cannot store more measurements"};

        package[idx++] = std::move(m);
    }

    //! Packages all the stored measurements, and remove the stored measurements.
    Package package_and_clear() override
    {
        idx = 0;
        return package;
    }

  private:
    Package package{};
    unsigned idx{0};
};

#endif /* BASIC_ARRAY_STORE_HPP */

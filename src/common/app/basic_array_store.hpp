/**
 * @file        basic_array_store.hpp
 * @brief       A Store implementation which buffers the measurements in an std::array.
 */
#ifndef BASIC_ARRAY_STORE_HPP
#define BASIC_ARRAY_STORE_HPP

#include <array>
#include <iterator>
#include <stdexcept>
#include <vector>

#include "interfaces/store.hpp"

template<typename Measurement, unsigned MaxMeasurements>
struct BasicArrayStore : Store<Measurement, std::vector<Measurement>>
{
    using Buffer = std::array<Measurement, MaxMeasurements>;
    using Package = std::vector<Measurement>;

    void store(Measurement m) override
    {
        if (idx == buffer.size())
            throw std::runtime_error{"Cannot store more measurements"};

        buffer[idx++] = std::move(m);
    }

    //! Packages all the stored measurements, and remove the stored measurements.
    Package package_and_clear() override
    {
        auto beg{std::cbegin(buffer)};
        auto end{std::next(beg, idx)};
        idx = 0;
        return Package(beg, end);
    }

  private:
    Buffer buffer{};
    unsigned idx{0};
};

#endif /* BASIC_ARRAY_STORE_HPP */

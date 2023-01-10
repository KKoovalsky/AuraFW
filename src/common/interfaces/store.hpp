/**
 * @file        store.hpp
 * @brief       store.hpperface.
 */
#ifndef STORE_HPP
#define STORE_HPP

template<typename Measurement, typename Package>
struct Store
{
    //! Stores a single measurement.
    virtual void store(Measurement) = 0;

    //! Packages all the stored measurements, and remove the stored measurements.
    virtual Package package_and_clear() = 0;

    virtual ~Store() = default;
};

#endif /* STORE_HPP */

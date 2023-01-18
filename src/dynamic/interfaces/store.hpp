/**
 * @file        store.hpp
 * @brief       Store interface.
 */
#ifndef STORE_HPP
#define STORE_HPP

template<typename Measurement, typename Package>
struct Store
{
    virtual void store(Measurement) = 0;
    virtual Package package_and_clear() = 0;
    virtual ~Store() = default;
};

#endif /* STORE_HPP */

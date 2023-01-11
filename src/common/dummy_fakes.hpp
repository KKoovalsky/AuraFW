/**
 * @file        dummy_fakes.hpp
 * @brief       Dummy fake types for easier testing and simulation.
 */
#ifndef DUMMY_FAKES_HPP
#define DUMMY_FAKES_HPP

#include "interfaces/measurer.hpp"
#include "interfaces/sender.hpp"
#include "interfaces/store.hpp"

struct DummyMeasurement
{
};

struct DummyPackage
{
};

struct DummyEvent
{
    int value{0};
};

template<typename Measurement, typename Package>
struct DummyStore : Store<Measurement, Package>
{
    void store(Measurement) override
    {
    }

    Package package_and_clear() override
    {
        return {};
    }
};

template<typename T>
struct DummyMeasurer : Measurer<T>
{
    T measure() override
    {
        return {};
    }
};

template<typename T>
struct DummySender : Sender<T>
{
    void send_async(T) override
    {
    }
};

#endif /* DUMMY_FAKES_HPP */

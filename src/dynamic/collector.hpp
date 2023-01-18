/**
 * @file        collector.hpp
 */
#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <utility>

#include "interfaces/measurer.hpp"
#include "interfaces/observer.hpp"
#include "interfaces/sender.hpp"
#include "interfaces/store.hpp"

#include "events.hpp"

template<typename Measurement, typename Package>
struct Collector : Observer<Event::MeasurementInterval>, Observer<Event::SendingInterval>
{
    explicit Collector();

    Collector(Store<Measurement, Package>& store, Measurer<Measurement>& measurer, Sender<Package>& sender) :
        store(store), measurer(measurer), sender(sender)
    {
    }

    void update(const Event::MeasurementInterval&) override
    {
        auto measurement{measurer.measure()};
        store.store(std::move(measurement));
    }

    void update(const Event::SendingInterval&) override
    {
        auto package{store.package_and_clear()};
        sender.send(std::move(package));
    }

  private:
    Store<Measurement, Package>& store;
    Measurer<Measurement>& measurer;
    Sender<Package>& sender;
};

#endif /* COLLECTOR_HPP */

/**
 * @file        collector.hpp
 * @brief       Collects measurements and sends them on sending interval.
 */
#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <utility>

#include "events.hpp"

#include "interfaces/measurer.hpp"
#include "interfaces/sender.hpp"
#include "interfaces/store.hpp"

// TODO: Remember to register me to MeasurementInterval and SendingInterval events.
// TODO: Weirdness: why MeasurementInterval is an event, as only this class will be using this event?
// TODO: Weirdness: use of the Sender::send_async() is weird.
template<typename Measurement, typename Package>
class Collector
{
  public:
    using Store_ = Store<Measurement, Package>;
    using Measurer_ = Measurer<Measurement>;

    // FIXME: Telling explicitly that this is async breaks the rule of separation of concerns. The Collector does
    // not only react to the events, but it also knows stuff about RT.
    using NonBlockingSender = Sender<Package>;

    explicit Collector(Store_& store, Measurer_& measurer, NonBlockingSender& sender) :
        store{store}, measurer{measurer}, sender{sender}
    {
    }

    void update(const Event::MeasurementInterval&)
    {
        auto measurement{measurer.measure()};
        store.store(std::move(measurement));
    }

    void update(const Event::SendingInterval&)
    {
        auto package{store.package_and_clear()};
        sender.send(std::move(package));
    }

  private:
    Store_& store;
    Measurer_& measurer;
    NonBlockingSender& sender;
};

#endif /* COLLECTOR_HPP */

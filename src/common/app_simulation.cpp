/**
 * @file	app_simulation.cpp
 * @brief	App simulation which uses static polymporphizm where possible.
 */

#include <iostream>

#include "collector_using_static_polymorphizm.hpp"

#include "app/basic_array_store.hpp"

#include "events.hpp"
#include "utils/signal_std_tuple_based_with_compile_time_connections.hpp"

struct Measurement
{
    unsigned id{0};
};

struct Measurer
{
    Measurement measure()
    {
        auto id{measurement_id++};
        std::cout << "MEASURING: " << id << std::endl;
        return {.id = id};
    }

    unsigned measurement_id{0};
};

static constexpr unsigned max_measurements{10};
using Store_ = BasicArrayStore<Measurement, max_measurements>;
using Package = Store_::Package;

struct Sender
{
    void send(const Package& package)
    {
        std::cout << "SENDING: ";
        for (auto e : package)
            std::cout << e.id << ", ";
        std::cout << std::endl;
    }
};

template<typename Sender>
struct SenderDecorator
{
    explicit SenderDecorator(Sender& sender) : sender{sender}
    {
    }

    void send(Package&& package)
    {
        std::cout << "DECORATED SENDER" << std::endl;
        sender.send(std::move(package));
    }

    Sender& sender;
};

template<typename Event, typename Signal>
struct SignalDecorator
{
    explicit SignalDecorator(Signal& signal) : signal{signal}
    {
    }

    void notify(Event evt)
    {
        std::cout << "DECORATED SIGNAL" << std::endl;
        signal.notify(std::move(evt));
    }

    Signal& signal;
};

struct Backgrounder
{
    void notify(Event::MeasurementInterval evt)
    {
    }
};

int main()
{
    Store_ store;
    Measurer measurer;
    Sender sender;

    SenderDecorator sender_decorator{sender};
    CollectorWhichUsesStaticPolymporphizm<Measurement, Package, Store_, Measurer, decltype(sender_decorator)> collector{
        store, measurer, sender_decorator};

    SignalStdTupleBasedWithCompileTimeConnections<Event::MeasurementInterval, decltype(collector)>
        measurement_interval_signal{collector};
    SignalStdTupleBasedWithCompileTimeConnections<Event::SendingInterval, decltype(collector)> sending_interval_signal{
        collector};

    SignalDecorator<Event::MeasurementInterval, decltype(measurement_interval_signal)>
        measurement_interval_signal_decorator{measurement_interval_signal};

    for (unsigned i{0}; i < 10; ++i)
    {
        measurement_interval_signal_decorator.notify({});
        if (i % 3 == 0)
            sending_interval_signal.notify({});
    }

    return 0;
}
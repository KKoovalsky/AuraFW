/**
 * @file	app_simulation.cpp
 * @brief	App simulation which uses static polymporphizm where possible.
 */

#include <iostream>
#include <utility>

#include "app/basic_array_store.hpp"
#include "collector_using_static_polymorphizm.hpp"
#include "events.hpp"
#include "utils/active_with_compile_time_actors.hpp"
#include "utils/signal_std_tuple_based_with_compile_time_connections.hpp"

#include "jungles_os_helpers/native/message_pump.hpp"
#include "jungles_os_helpers/native/thread2.hpp"

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
    void send(const Package& package) // NOLINT
    {
        std::cout << "SENDING: ";
        for (auto e : package)
            std::cout << e.id << ", ";
        std::cout << std::endl;
    }
};

template<typename Sender>
struct SenderDecoratorDynamicLike
{
    explicit SenderDecoratorDynamicLike(Sender& sender) : sender{sender}
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

template<typename Event, typename Signal>
struct SignalDecorator2 : Signal
{
    template<typename... Args>
    explicit SignalDecorator2(Args&&... args) : Signal(std::forward<Args>(args)...)
    {
    }

    void notify(Event evt)
    {
        std::cout << "SIGNAL DECORATOR2" << std::endl;
        static_cast<Signal*>(this)->notify(std::move(evt));
    }
};

struct Backgrounder
{
    void notify(Event::MeasurementInterval evt)
    {
    }
};

template<typename Event, typename... Listeners>
using Signal = SignalStdTupleBasedWithCompileTimeConnections<Event, Listeners...>;

template<auto MethodPointer>
using Thread = jungles::native::thread2<MethodPointer>;

template<typename Message>
using MessagePump = jungles::native::message_pump<Message>;

template<typename Measurement, typename Package, typename Store, typename Measurer, typename Sender>
using Collector = CollectorWhichUsesStaticPolymporphizm<Measurement, Package, Store, Measurer, Sender>;

int main()
{
    Store_ store;
    Measurer measurer;
    Sender sender;

    SenderDecoratorDynamicLike sender_decorator{sender};

    Collector<Measurement, Package, Store_, Measurer, decltype(sender_decorator)> collector{
        store, measurer, sender_decorator};

    Signal<Event::MeasurementInterval, decltype(collector)> measurement_interval_signal{collector};
    Signal<Event::SendingInterval, decltype(collector)> sending_interval_signal{collector};

    auto active{make_active_with_compile_time_actors<Thread, MessagePump>(
        [&](Event::MeasurementInterval e) { measurement_interval_signal.notify(e); },
        [&](Package p) { sender.send(p); })};

    for (unsigned i{0}; i < 10; ++i)
    {
        active.send(Event::MeasurementInterval{});
        if (i % 3 == 0)
            sending_interval_signal.notify({});
    }

    return 0;
}

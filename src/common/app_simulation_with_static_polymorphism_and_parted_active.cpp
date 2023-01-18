/**
 * @file	app_simulation.cpp
 * @brief	App simulation which uses static polymporphizm where possible.
 */

#include <iostream>
#include <utility>

#include "app/basic_array_store.hpp"
#include "collector_using_static_polymorphizm.hpp"
#include "events.hpp"
#include "native_timer.hpp"
#include "utils/active_with_compile_time_actors_parted.hpp"
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

template<typename Event, typename... Listeners>
using Signal = SignalStdTupleBasedWithCompileTimeConnections<Event, Listeners...>;

template<auto MethodPointer>
using Thread = jungles::native::thread2<MethodPointer>;

template<typename Message>
using MessagePump = jungles::native::message_pump<Message>;

template<typename Measurement, typename Package, typename Store, typename Measurer, typename Sender>
using Collector = CollectorWhichUsesStaticPolymporphizm<Measurement, Package, Store, Measurer, Sender>;

// FIXME: Package shall be SendPackage
using Dispatcher_ = Dispatcher<MessagePump, Event::MeasurementInterval, Event::SendingInterval, Package>;

using namespace std::chrono_literals;

struct SenderDecorator
{
    explicit SenderDecorator(Dispatcher_& dispatcher) : dispatcher{dispatcher}
    {
    }

    void send(Package&& package)
    {
        dispatcher.send(std::move(package));
    }

    Dispatcher_& dispatcher;
};

template<typename Event>
struct EventNotifier
{
    explicit EventNotifier(Dispatcher_& d) : dispatcher{d}
    {
    }

    void notify()
    {
        dispatcher.send(Event{});
    }

  private:
    Dispatcher_& dispatcher;
};

int main()
{
    Store_ store;
    Measurer measurer;
    Sender sender;

    Dispatcher_ dispatcher;
    SenderDecorator sender_decorator{dispatcher};

    Collector<Measurement, Package, Store_, Measurer, decltype(sender_decorator)> collector{
        store, measurer, sender_decorator};

    Signal<Event::MeasurementInterval, decltype(collector)> measurement_interval_signal{collector};
    Signal<Event::SendingInterval, decltype(collector)> sending_interval_signal{collector};

    auto active{
        dispatcher.make_active<Thread>([&](Event::MeasurementInterval&& e) { measurement_interval_signal.notify(e); },
                                       [&](Event::SendingInterval&& e) { sending_interval_signal.notify(e); },
                                       [&](Package&& p) { sender.send(p); })};

    EventNotifier<Event::MeasurementInterval> measurement_interval_notifier{dispatcher};
    EventNotifier<Event::SendingInterval> sending_interval_notifier{dispatcher};

    concurrencpp::runtime runtime;
    NativeTimer measurement_timer{measurement_interval_notifier, runtime, 100ms};
    NativeTimer sending_timer{sending_interval_notifier, runtime, 300ms};

    std::this_thread::sleep_for(2s);

    return 0;
}

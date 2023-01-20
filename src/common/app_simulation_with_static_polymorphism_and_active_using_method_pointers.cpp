/**
 * @file	app_simulation.cpp
 * @brief	App simulation which uses static polymporphizm where possible.
 */

#include <iostream>
#include <utility>

#include "app/basic_array_store.hpp"
#include "collector_using_static_polymorphizm.hpp"
#include "events.hpp"
#include "utils/active_with_compile_time_actors_being_method_pointers.hpp"
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
using Package_ = Store_::Package;

struct Sender
{
    void send(const Package_& package) // NOLINT
    {
        std::cout << "SENDING: ";
        for (auto e : package)
            std::cout << e.id << ", ";
        std::cout << std::endl;
    }
};

template<typename Package>
struct SendRequest
{
    Package package;
};

template<typename Package, typename Active>
struct DispatchingSender
{
    explicit DispatchingSender(Active& active) : active{active}
    {
    }

    void send(Package&& package)
    {
        active.send(std::move(package));
    }

    Active& active;
};

template<typename Package, typename Sender>
struct SendRequestHandler
{
    explicit SendRequestHandler(Sender& sender) : sender(sender)
    {
    }

    void handle(SendRequest<Package>&& req)
    {
        sender.send(std::move(req));
    }

    Sender& sender;
};

template<typename Event, typename... Listeners>
using Signal = SignalStdTupleBasedWithCompileTimeConnections<Event, Listeners...>;

template<auto MethodPointer>
using Thread = jungles::native::thread2<MethodPointer>;

template<typename Message>
using MessagePump = jungles::native::message_pump<Message>;

// using SendRequestHandler_ = SendRequestHandler<Package_, Sender>;
// using Active_ = experimental::Active < Thread, MessagePump, &SendRequestHandler_::handle,

// using Collector = CollectorWhichUsesStaticPolymporphizm<Measurement, Package_, Store_, Measurer,
// DispatchingSender<Package_, Active>;

int main()
{
    // Actor1 a1;
    // Actor2 a2;
    // Active_ a{a1, a2};
    //
    // a.send(Event1{.id = 10});
    // a.send(Event2{.id = 20});
    //
    // return 0;
    // Store_ store;
    // Measurer measurer;
    // Sender sender;
    //
    // SenderDecoratorDynamicLike sender_decorator{sender};
    //
    // Collector<Measurement, Package, Store_, Measurer, decltype(sender_decorator)> collector{
    //     store, measurer, sender_decorator};
    //
    // Signal<Event::MeasurementInterval, decltype(collector)> measurement_interval_signal{collector};
    // Signal<Event::SendingInterval, decltype(collector)> sending_interval_signal{collector};
    //
    // auto active{make_active_with_compile_time_actors<Thread, MessagePump>(
    //     [&](Event::MeasurementInterval e) { measurement_interval_signal.notify(e); },
    //     [&](Package p) { sender.send(p); })};
    //
    // for (unsigned i{0}; i < 10; ++i)
    // {
    //     active.send(Event::MeasurementInterval{});
    //     if (i % 3 == 0)
    //         sending_interval_signal.notify({});
    // }
    //
    return 0;
}

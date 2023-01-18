#include <concurrencpp/runtime/runtime.h>
#include <iostream>

#include "active_impl.hpp"
#include "basic_array_store.hpp"
#include "collector.hpp"
#include "events.hpp"
#include "message_pump_impl.hpp"
#include "native_timer.hpp"
#include "signal.hpp"
#include "thread_impl.hpp"

#include "interfaces/active.hpp"

struct Measurement
{
    int id{0};
};

using Store_ = BasicArrayStore<Measurement, 10>;
using Package = Store_::Package;

struct MeasurerImpl : Measurer<Measurement>
{
    Measurement measure() override
    {
        auto id{measurement_id++};
        std::cout << "MEASURING: " << id << std::endl;
        return {.id = id++};
    }

    int measurement_id{0};
};

struct SenderImpl : Sender<Package>
{
    void send(Package package) override
    {
        std::cout << "SENDING: ";
        for (auto e : package)
            std::cout << e.id << ", ";
        std::cout << std::endl;
    }
};

struct SenderDecorator : Sender<Package>
{
    explicit SenderDecorator(Sender<Package>& sender, Active& active) : decorated{sender}, active{active}
    {
    }

    void send(Package package) override
    {
        active.send([&, package = std::move(package)]() { decorated.send(package); });
    }

    Sender<Package>& decorated;
    Active& active;
};

template<typename Event>
struct NotifiableDecorator : Notifiable<Event>
{
    explicit NotifiableDecorator(Notifiable<Event>& notifiable, Active& active) : notifiable{notifiable}, active{active}
    {
    }

    void notify(const Event& e) override
    {
        active.send([&, e]() { notifiable.notify(e); });
    }

    Notifiable<Event>& notifiable;
    Active& active;
};

using namespace std::chrono_literals;

int main()
{
    ThreadImpl thread;
    MessagePumpImpl message_pump;
    ActiveImpl active{thread, message_pump};

    Store_ store;
    MeasurerImpl measurer;
    SenderImpl sender;

    SenderDecorator sender_decorator{sender, active};

    Collector collector{store, measurer, sender};

    Signal<Event::MeasurementInterval> measurement_interval_signal;
    Signal<Event::SendingInterval> sending_interval_signal;

    measurement_interval_signal.add(&collector);
    sending_interval_signal.add(&collector);

    NotifiableDecorator measurement_interval_signal_decorator{measurement_interval_signal, active};
    NotifiableDecorator sending_interval_signal_decorator{sending_interval_signal, active};

    concurrencpp::runtime runtime;
    NativeTimer<Event::MeasurementInterval> measurement_timer{measurement_interval_signal_decorator, runtime, 100ms};
    NativeTimer<Event::SendingInterval> sending_timer{sending_interval_signal_decorator, runtime, 300ms};

    std::this_thread::sleep_for(2s);

    return 0;
}

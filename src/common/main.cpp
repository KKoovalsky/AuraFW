/**
 * @file	main.cpp
 * @brief
 */
#include <iostream>

#include "collector.hpp"
#include "dummy_fakes.hpp"

#include "app/basic_array_store.hpp"

#include "utils/signal.hpp"

struct DummyListener : Listener<DummyEvent>
{
    explicit DummyListener(unsigned id) : id{id}
    {
    }

    void update(const DummyEvent& evt) override
    {
        std::cout << "DummyListener ID: " << id << ", event: " << evt.value << std::endl;
    }

  private:
    unsigned id;
};

int main()
{
    constexpr unsigned max_measurements{10};
    using Store = BasicArrayStore<DummyMeasurement, max_measurements>;
    using Package = Store::Package;

    Store store;
    DummyMeasurer<DummyMeasurement> measurer;
    DummySender<Package> sender;
    Collector<DummyMeasurement, Package> collector{store, measurer, sender};

    Signal<DummyEvent> signal;
    DummyListener l1{1};
    DummyListener l2{2};

    signal.add(&l1);
    signal.add(&l2);

    signal.notify({.value = 10});
    signal.notify({.value = 20});

    // TODO: We need a timer which counts measurement intervals, and dispatches the event.
    // TODO: We need a timer which counts sending intervals, and dispatches the event.

    // TODO: We need two event buses, one for measurement interval, and second for sending interval.
    // TODO: We need an active object which dispatches the events in a single thread.
    // TODO: We need the Sender state machine which sends immediately if idle, or stores the packages if not idle.

    return 0;
}

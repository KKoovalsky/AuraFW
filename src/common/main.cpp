/**
 * @file	main.cpp
 * @brief
 */

#include "collector.hpp"
#include "dummy_fakes.hpp"

int main()
{
    DummyStore<DummyMeasurement, DummyPackage> store;
    DummyMeasurer<DummyMeasurement> measurer;
    DummySender<DummyPackage> sender;
    Collector<DummyMeasurement, DummyPackage> collector{store, measurer, sender};

    // TODO: We need a timer which counts measurement intervals, and dispatches the event.
    // TODO: We need a timer which counts sending intervals, and dispatches the event.

    // TODO: We need two event buses, one for measurement interval, and second for sending interval.
    // TODO: We need an active object which dispatches the events in a single thread.
    // TODO: We need the Sender state machine which sends immediately if idle, or stores the packages if not idle.

    return 0;
}

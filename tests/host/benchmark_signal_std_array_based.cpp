/**
 * @file        benchmark_signal_std_array_based.cpp
 */
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "utils/signal_std_array_based_with_compile_time_connections.hpp"

struct Event
{
};

struct Listener1 : Listener<Event>
{
    void update(const Event&) override
    {
        ++num_called;
    }

    unsigned num_called{0};
};

struct Listener2 : Listener<Event>
{
    void update(const Event&) override
    {
        ++num_called;
    }

    unsigned num_called{0};
};

struct Listener3 : Listener<Event>
{
    void update(const Event&) override
    {
        ++num_called;
    }

    unsigned num_called{0};
};

TEST_CASE("Benchmark for SignalStdArrayBasedWithCompileTimeListeners", // NOLINT(cert-err58-cpp)
          "[SignalStdArrayBasedWithCompileTimeListenersBenchmark]")
{
    Listener1 l0, l1, l2;
    Listener2 l3, l4, l5;
    Listener3 l6, l7, l8, l9;

    BENCHMARK("Create")
    {
        return make_signal_std_array_based_with_compile_time_connections<Event>(l0, l1, l2, l3, l4, l5, l6, l7, l8, l9);
    };

    BENCHMARK_ADVANCED("Dispatch")(Catch::Benchmark::Chronometer meter)
    {
        Event evt;
        auto signal{
            make_signal_std_array_based_with_compile_time_connections<Event>(l0, l1, l2, l3, l4, l5, l6, l7, l8, l9)};
        meter.measure([&]() { signal.notify(evt); });
        REQUIRE(l0.num_called > 0);
        REQUIRE(l6.num_called > 0);
        REQUIRE(l9.num_called > 0);
    };
}

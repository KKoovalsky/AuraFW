/**
 * @file        test_signal_with_compile_time_connections.cpp
 * @brief       Benchmarking test of a Signal implementations.
 */
#include <catch2/catch_test_macros.hpp>

#include "utils/signal_std_array_based_with_compile_time_connections.hpp"
#include "utils/signal_std_tuple_based_with_compile_time_connections.hpp"

struct Event
{
    int id{0};
};

template<unsigned Id>
struct ListenerImpl : Listener<Event>
{
    void update(const Event&) override
    {
        ++num_called;
    }

    unsigned num_called{0};
};

TEST_CASE("Signals are dispatched, for std::array based signal", // NOLINT(cert-err58-cpp)
          "[SignalStdArrayBasedWithCompileTimeListeners]")
{
    ListenerImpl<1> l0, l1, l2, l3, l4;
    ListenerImpl<2> l5, l6, l7, l8, l9;

    SECTION("Dispatches the event")
    {
        auto signal{
            make_signal_std_array_based_with_compile_time_connections<Event>(l0, l1, l2, l3, l4, l5, l6, l7, l8, l9)};
        signal.notify({});
        REQUIRE(l0.num_called == 1);
        REQUIRE(l5.num_called == 1);
        REQUIRE(l9.num_called == 1);
    }
}

/**
 * @defgroup ListenersForStdTupleTest	Multiple listeners defined to provide various types to the std::tuple.
 * @{
 */
struct Listener1
{
    void update(const Event&)
    {
        ++num_called;
    }

    unsigned num_called{0};
};

struct Listener2
{
    void update(const Event&)
    {
        ++num_called;
    }

    unsigned num_called{0};
};

struct Listener3
{
    void update(const Event&)
    {
        ++num_called;
    }

    unsigned num_called{0};
};

/** @} */ // End of group  ListenersForStdTupleTest

TEST_CASE("Signals are dispatched, for std::tuple based signal", // NOLINT(cert-err58-cpp)
          "[SignalStdTupleBasedWithCompileTimeListeners]")
{
    Listener1 l0, l1, l2;
    Listener2 l3, l4, l5;
    Listener3 l6, l7, l8, l9;

    SECTION("Dispatches the event")
    {
        auto signal{
            make_signal_std_tuple_based_with_compile_time_connections<Event>(l0, l1, l2, l3, l4, l5, l6, l7, l8, l9)};
        signal.notify({});
        REQUIRE(l0.num_called == 1);
        REQUIRE(l5.num_called == 1);
        REQUIRE(l9.num_called == 1);
    }
}

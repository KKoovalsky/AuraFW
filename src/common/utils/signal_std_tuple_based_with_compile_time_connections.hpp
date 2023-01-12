/**
 * @file        signal_compile_time.hpp
 * @brief       A Signal implementation which gets observers at the compile-time.
 */
#ifndef SIGNAL_STD_TUPLE_BASED_WITH_COMP_TIME_CONNS
#define SIGNAL_STD_TUPLE_BASED_WITH_COMP_TIME_CONNS

#include <tuple>

// TODO: Concept for the Listeners: all shall implement update(Event)
template<typename Event, typename... Listeners>
struct SignalStdTupleBasedWithCompileTimeConnections
{
    constexpr explicit SignalStdTupleBasedWithCompileTimeConnections(Listeners&... ls) : listeners{&ls...}
    {
    }

    void notify(Event evt) const
    {
        // Call each listener
        std::apply([&evt](auto&... ls) { (..., ls->update(evt)); }, listeners);
    }

  private:
    const std::tuple<Listeners*...> listeners;
};

template<typename Event, typename... Listeners>
inline constexpr auto make_signal_std_tuple_based_with_compile_time_connections(Listeners&... ls)
{
    return SignalStdTupleBasedWithCompileTimeConnections<Event, Listeners...>(ls...);
};

#endif /* SIGNAL_STD_TUPLE_BASED_WITH_COMP_TIME_CONNS */

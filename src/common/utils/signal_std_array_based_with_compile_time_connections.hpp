/**
 * @file        signal_compile_time.hpp
 * @brief       A Signal implementation which gets observers at the compile-time.
 */
#ifndef SIGNAL_STD_ARRAY_BASED_WITH_COMP_TIME_CONNS
#define SIGNAL_STD_ARRAY_BASED_WITH_COMP_TIME_CONNS

#include <array>

#include "interfaces/listener.hpp"

// TODO: Concept for the Listeners: all shall implement Listener interface!
template<typename Event, typename... Listeners>
struct SignalStdArrayBasedWithCompileTimeConnections
{
    using Listener_ = Listener<Event>;
    static inline constexpr unsigned NumberOfListeners{sizeof...(Listeners)};

    constexpr explicit SignalStdArrayBasedWithCompileTimeConnections(Listeners&... ls) : listeners{&ls...}
    {
    }

    void notify(Event evt) const
    {
        for (auto l : listeners)
            l->update(evt);
    }

  private:
    const std::array<Listener_*, NumberOfListeners> listeners{};
};

template<typename Event, typename... Listeners>
inline constexpr auto make_signal_std_array_based_with_compile_time_connections(Listeners&... ls)
{
    return SignalStdArrayBasedWithCompileTimeConnections<Event, Listeners...>(ls...);
};

#endif /* SIGNAL_STD_ARRAY_BASED_WITH_COMP_TIME_CONNS */

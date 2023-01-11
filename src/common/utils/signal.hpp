/**
 * @file        signal.hpp
 * @brief       Signal template, cross-platform.
 */
#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <array>
#include <functional>
#include <stdexcept>

enum class DispatchPolicy
{
    //! Dispatch within a single call to the Dispatcher.
    all_at_once,

    //! Call the Dispatcher once for each Listener.
    one_after_another
};

template<typename Event>
struct Listener
{
    virtual void update(const Event&) = 0;
    virtual ~Listener() = default;
};

static inline void ImmediateDispatcher(const std::function<void(void)>& dispatched_action)
{
    dispatched_action();
}

// TODO: Make the Listeners only addable through variadic template parameters. This assumes the signal is available
// for the entire life cycle of the program. May use concepts to check if Listeners implement some certain behavior.
// Do not care about the code bloat, since for each Event the template is anew.
// TODO: Add a concept, or a static assertions (may be easier), for the Dispatcher. It is a callable (invocable),
// which takes std::function as a parameter.
// TODO: Can we skip for the Dispatcher to take the std::function, and use direct lambda as it's tempate type?
//! @note: Assume add() and notify() are not called concurrently.
template<typename Event,
         unsigned MaxListeners = 4,
         auto Dispatcher = ImmediateDispatcher,
         DispatchPolicy DispatchPolicy_ = DispatchPolicy::all_at_once>
struct Signal
{
    using Listener_ = Listener<Event>;

    void add(Listener_* listener)
    {
        if (idx >= listeners.size())
            throw std::runtime_error{"No more listeners can be registered"};

        listeners[idx++] = listener;
    }

    void notify(Event evt)
    {
        if constexpr (DispatchPolicy_ == DispatchPolicy::all_at_once)
        {
            Dispatcher([evt = std::move(evt), this]() {
                for (unsigned i{0}; i < idx; ++i)
                    listeners[i]->update(evt);
            });
        } else
        {
            // FIXME: May involve a lot of copying. Use std::shared_ptr<const Event> here.
            for (unsigned i{0}; i < idx; ++i)
                Dispatcher([evt, this, i]() { listeners[i]->update(evt); });
        }
    }

  private:
    //! Use std::array to avoid heap allocation on embedded machines.
    std::array<Listener_*, MaxListeners> listeners{};
    unsigned idx{0};
};

#endif /* SIGNAL_HPP */

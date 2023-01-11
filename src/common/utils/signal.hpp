/**
 * @file        signal.hpp
 * @brief       Signal template, cross-platform.
 *
 * @note        It may be beneficial to use ImmediateDispatcher, if most of the Listeners are quick, but there is one,
 *              or two, which may block the thread context for a long time. In such a case it would be good to dispatch
 *              the async job within the long-running Listener's update() function itself.
 */
#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <array>
#include <functional>
#include <stdexcept>

enum class DispatchPolicy
{
    /** @brief Dispatch within a single call to the Dispatcher.
     *
     * It means that on notify() the Dispatcher will be called once, with a job of calling each Listener successively.
     *
     * It is useful when you want to avoid dispatching the event from the notifier thread context.
     *
     * Use it when all of the Listeners return quickly from update(), thus they don't occupy a lot of time, so the
     * impression of immediate (and parallel) handling of the event by all the Listeners can be preserved.
     *
     * When ImmediateDispatcher is used with this policy, all the Listeners will be called immediately within the
     * notify() function. (For ImmediateDispatcher the DispatchPolicy doesn't do anything).
     */
    all_at_once,

    /** @brief Call the Dispatcher once for each Listener.
     *
     * It means that on notify() the Dispatcher will be called per each Listener, to run its update().
     *
     * Use it when you want to implement thread pool design pattern. It will allow to dispatch each update() in a
     * separate runner.
     *
     * When ImmediateDispatcher is used with this policy, all the Listeners will be called immediately within the
     * notify() function. (For ImmediateDispatcher the DispatchPolicy doesn't do anything).
     */
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

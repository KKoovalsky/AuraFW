/**
 * @file        active_with_compile_time_actors.hpp
 * @brief       An Active Object which knows the Actor functors at compile time.
 */
#ifndef ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP
#define ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP

#include <tuple>
#include <utility>
#include <variant>

#include "utils/function_traits.hpp"

// FIXME: Thread must be also a template, with a start() function that takes a lambda.
template<typename Thread, template<typename> typename MessagePumpTemplate, typename... Actors>
class Active
{
  public:
    static_assert(((util::function_traits<Actors>::arity == 1) and ...),
                  "All of the Actors must take only one parameter!");

    // FIXME: static_assert that Actors take unique types. Can be checked through MessageTypes!

    // using Message = std::function<void()>;

    Active(const Active&) = delete;
    Active& operator=(const Active&) = delete;
    Active(Active&&) noexcept = default;
    Active& operator=(Active&&) noexcept = default;

    template<typename... Ts>
    explicit Active(Ts&&... as) : actors{std::forward<Ts>(as)...}
    {
        thread.start([&]() { run(); });
    }

    ~Active()
    {
        // send([&] { done = true; });
        thread.join();
    }

    template<typename Msg>
    void send(Msg&& msg)
    {
        message_pump.send(std::forward<Msg>(msg));
    }

  private:
    // FIXME: Use remove_reference to be able to switch between reference and const-reference.
    using MessageTypes = std::tuple<typename util::function_traits<Actors>::template argument<0>...>;
    using Message = std::variant<typename util::function_traits<Actors>::template argument<0>...>;
    using MessagePump = MessagePumpTemplate<Message>;

    void run()
    {
        while (!done)
        {
            // auto msg{message_pump.receive()};
            // msg(); // note: last message sets done to true
        }
    }

    bool done{false};
    MessagePump message_pump;
    Thread thread;
    const std::tuple<Actors...> actors;
};

template<typename Thread, template<typename> typename MessagePumpTemplate, typename... Actors>
inline auto make_active_with_compile_time_actors(Actors&&... actors)
{
    return Active<Thread, MessagePumpTemplate, Actors...>(std::forward<Actors>(actors)...);
}

#endif /* ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP */

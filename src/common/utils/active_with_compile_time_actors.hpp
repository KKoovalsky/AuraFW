/**
 * @file        active_with_compile_time_actors.hpp
 * @brief       An Active Object which knows the Actor functors at compile time.
 */
#ifndef ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP
#define ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP

#include <tuple>
#include <utility>

// FIXME: Thread must be also a template, with a start() function that takes a lambda.
template<typename Thread, template<typename> typename MessagePumpTemplate, typename... Actors>
class Active
{
  public:
    // using Message = std::function<void()>;
    // using MessagePump = MessagePumpTemplate<Message>;

    Active(const Active&) = delete;
    Active& operator=(const Active&) = delete;
    Active(Active&&) noexcept = default;
    Active& operator=(Active&&) noexcept = default;

    explicit Active(Actors&&... as) : actors{std::forward<Actors>(as)...}
    {
        thread.start([&]() { run(); });
    }

    ~Active()
    {
        // send([&] { done = true; });
        thread.join();
    }

    // void send(Message m)
    // {
    //     message_pump.send(std::move(m));
    // }

  private:
    void run()
    {
        while (!done)
        {
            // auto msg{message_pump.receive()};
            // msg(); // note: last message sets done to true
        }
    }

  public:
    bool done{false};
    // MessagePump message_pump;
    Thread thread;
    std::tuple<Actors...> actors;
};

template<typename Thread, template<typename> typename MessagePumpTemplate, typename... Actors>
inline auto make_active_with_compile_time_actors(Actors&&... actors)
{
    return Active<Thread, MessagePumpTemplate, Actors...>(std::forward<Actors>(actors)...);
}

#endif /* ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP */

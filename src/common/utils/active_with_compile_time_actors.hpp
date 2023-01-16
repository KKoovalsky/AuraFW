/**
 * @file        active_with_compile_time_actors.hpp
 * @brief       An Active Object which knows the Actor functors at compile time.
 */
#ifndef ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP
#define ACTIVE_WITH_COMPILE_TIME_ACTORS_HPP

#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include "utils/function_traits.hpp"

namespace detail
{
template<typename T, typename U, typename... Us>
static inline constexpr int get_index()
{
    if constexpr (std::is_same_v<T, U>)
        return 0;
    else
    {
        static_assert(sizeof...(Us) > 0, "Type not found in the tuple");
        return get_index<T, Us...>() + 1;
    }
    return 0;
}

template<typename T, typename U, typename... Us>
static inline constexpr int get_index(const std::tuple<U, Us...>&)
{
    return get_index<T, U, Us...>();
}
} // namespace detail

// FIXME: Thread must be also a template, with a start() function that takes a lambda.
template<typename Thread, template<typename> typename MessagePumpTemplate, typename... Actors>
class Active
{
  public:
    static_assert(((util::function_traits<Actors>::arity == 1) and ...),
                  "All of the Actors must take only one parameter!");

    // FIXME: static_assert that Actors take unique types. Can be checked through MessageTypes!

    Active(const Active&) = delete;
    Active& operator=(const Active&) = delete;
    Active(Active&&) noexcept = default;
    Active& operator=(Active&&) noexcept = default;

  private:
    struct Done
    {
    };

  public:
    template<typename... Ts>
    explicit Active(Ts&&... as) : actors{std::forward<Ts>(as)...}
    {
        thread.start([&]() { run(); });
    }

    ~Active()
    {
        send(Done{});
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
    using Message = std::variant<Done, typename util::function_traits<Actors>::template argument<0>...>;
    using MessagePump = MessagePumpTemplate<Message>;

    void run()
    {
        while (!done)
        {
            auto msg{message_pump.receive()};
            std::visit(
                [&](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, Done>)
                    {
                        done = true;
                    } else
                    {
                        constexpr unsigned ActorIndex{detail::get_index<T>(MessageTypes{})};
                        auto& actor{std::get<ActorIndex>(actors)};
                        actor(std::forward<decltype(arg)>(arg));
                    }
                },
                std::move(msg));
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


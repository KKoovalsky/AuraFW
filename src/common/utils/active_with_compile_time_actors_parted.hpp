/**
 * @file        active_with_compile_time_actors_parted.hpp
 * @brief       An Active Object with the Dispatcher and Active object splitted.
 */
#ifndef ACTIVE_WITH_COMPILE_TIME_ACTORS_PARTED_HPP
#define ACTIVE_WITH_COMPILE_TIME_ACTORS_PARTED_HPP

#include <variant>

#include "utils/function_traits.hpp"

namespace detail
{
template<typename T, typename U, typename... Us>
static inline constexpr unsigned get_index()
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
static inline constexpr unsigned get_index(const std::tuple<U, Us...>&)
{
    return get_index<T, U, Us...>();
}
} // namespace detail

template<typename MessagePump, typename Proxy, template<auto> typename ThreadTemplate, typename... Actors>
struct Active
{
    static_assert(((util::function_traits<Actors>::arity == 1) and ...),
                  "All of the Actors must take only one parameter!");

    // FIXME: static_assert that Actors take unique types. Can be checked through MessageTypes!

    Active(const Active&) = delete;
    Active& operator=(const Active&) = delete;
    Active(Active&&) noexcept = default;
    Active& operator=(Active&&) noexcept = default;

  private:
    using Done = typename Proxy::Done;
    using MessageTypes = typename Proxy::MessageTypes;

  public:
    template<typename... Ts>
    explicit Active(MessagePump& mp, Ts&&... as) : message_pump{mp}, actors{std::forward<Ts>(as)...}
    {
        thread.start(this);
    }

    ~Active()
    {
        message_pump.send(Done{});
        thread.join();
    }

  private:
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

    using Thread = ThreadTemplate<&Active::run>;

    bool done{false};
    MessagePump& message_pump;
    Thread thread;
    const std::tuple<Actors...> actors;
};

template<template<typename> typename MessagePumpTemplate, typename... Events>
class Dispatcher
{
    struct Proxy
    {
        struct Done
        {
        };

        using MessageTypes = std::tuple<Events...>;
        using Message = std::variant<Done, Events...>;
        using MessagePump = MessagePumpTemplate<Message>;
    };

    using Message = typename Proxy::Message;
    using MessagePump = typename Proxy::MessagePump;

  public:
    void send(Message message)
    {
        message_pump.send(std::move(message));
    }

    template<template<auto Method> typename ThreadTemplate, typename... Actors>
    auto make_active(Actors&&... actors)
    {
        return Active<MessagePump, Proxy, ThreadTemplate, Actors...>(message_pump, std::forward<Actors>(actors)...);
    }

  private:
    MessagePump message_pump;
};

#endif /* ACTIVE_WITH_COMPILE_TIME_ACTORS_PARTED_HPP */

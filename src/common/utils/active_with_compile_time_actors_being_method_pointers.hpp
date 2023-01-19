/**
 * @file        active_with_compile_time_actors.hpp
 * @brief       An Active Object which knows the Actor functors at compile time.
 */
#ifndef ACTIVE_WITH_COMPILE_TIME_ACTORS_BEING_METHOD_POINTERS_HPP
#define ACTIVE_WITH_COMPILE_TIME_ACTORS_BEING_METHOD_POINTERS_HPP

#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include "utils/function_traits.hpp"

namespace experimental
{
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

namespace detail
{

template<typename T>
struct ClassOf
{
};

template<typename ReturnType, typename Class>
struct ClassOf<ReturnType(Class::*)>
{
    using type = Class;
};

template<typename T>
concept Method = requires(T)
{
    typename ClassOf<T>::type;
};

} // namespace detail

namespace detail
{
template<unsigned N, typename... Ts>
using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

};

template<template<auto Method> typename ThreadTemplate,
         template<typename>
         typename MessagePumpTemplate,
         auto... ActorsAsPointersToMethods>
requires(detail::Method<decltype(ActorsAsPointersToMethods)>and...) class Active
{
  public:
    static_assert(((util::function_traits<decltype(ActorsAsPointersToMethods)>::arity == 1) and ...),
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

    template<auto Method>
    using Owner = typename detail::ClassOf<decltype(Method)>::type;

    // FIXME: Use remove_reference to be able to switch between reference and const-reference.
    template<auto Method>
    using ConcreteMessage = typename util::function_traits<decltype(Method)>::template argument<0>;

    using Message = std::variant<Done, ConcreteMessage<ActorsAsPointersToMethods>...>;
    using MessagePump = MessagePumpTemplate<Message>;

    using MessageTypes = std::tuple<ConcreteMessage<ActorsAsPointersToMethods>...>;

  public:
    explicit Active(Owner<ActorsAsPointersToMethods>&... owners_) : owners{&owners_...}
    {
        thread.start(this);
    }

    void send(Message msg)
    {
        message_pump.send(std::move(msg));
    }

    ~Active()
    {
        send(Done{});
        thread.join();
    }

  private:
    void run()
    {
        while (not done)
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
                        constexpr unsigned Idx{detail::get_index<T>(MessageTypes{})};
                        constexpr auto actor{std::get<Idx>(actors)};
                        const auto& owner{std::get<Idx>(owners)};
                        (owner->*actor)(std::forward<decltype(arg)>(arg));
                    }
                },
                std::move(msg));
        }
    }
    //
    using Thread = ThreadTemplate<&Active::run>;

    static constexpr std::tuple actors{ActorsAsPointersToMethods...};
    bool done{false};
    MessagePump message_pump;
    Thread thread;
    const std::tuple<Owner<ActorsAsPointersToMethods>*...> owners;
};

// template<template<auto> typename ThreadTemplate, template<typename> typename MessagePumpTemplate, typename... Actors>
// inline auto make_active_with_compile_time_actors(Actors&&... actors)
// {
//     return Active<ThreadTemplate, MessagePumpTemplate, Actors...>(std::forward<Actors>(actors)...);
// }
//
} // namespace experimental

#endif /* ACTIVE_WITH_COMPILE_TIME_ACTORS_BEING_METHOD_POINTERS_HPP */


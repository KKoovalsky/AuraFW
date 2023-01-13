/**
 * @file        test_active_with_compile_time_actors.hpp
 * @brief       Tests the Active with compile time Actors.
 */
#include <iostream>
#include <thread>

#include <catch2/catch_test_macros.hpp>

#include "jungles_os_helpers/native/message_pump.hpp"
#include "jungles_os_helpers/native/thread.hpp"

#include "utils/active_with_compile_time_actors.hpp"

struct Event1
{
    int value{0};
};

// template<typename Dispatcher>
// struct Actor1
// {
//     explicit Actor1(Dispatcher& dispatcher) : dispatcher{dispatcher}
//     {
//     }
//
//     void notify(Event1 e) // NOLINT
//     {
//         dispatcher.send([e = std::move(e)]() { std::cout << "Actor1, event: " << e.value << std::endl; });
//     }
//
//     void run(Event1&& e)
//     {
//     }
//
//     Dispatcher& dispatcher;
// };

struct Event2
{
    std::string value;
};

struct Event3
{
    float f;
};

template<typename Message>
using MessagePump = jungles::native::message_pump<Message>;

struct Backgrounder
{
    void event1_handler(Event1&& evt1)
    {
    }

    void event2_handler(Event2&& evt2)
    {
    }

    void event3_handler(Event3&& evt3)
    {
    }

    template<typename Event, typename Function>
    auto wrap(Function* f)
    {
        return [this, f](Event&& evt) {
            this->*f(std::move(evt));
        };
    }

    template<typename Callable>
    struct Functor
    {
        using type = Callable;
    };

    // Functor<decltype([&](Event1&& evt) { event1_handler(std::move(evt)); })> f;

    void fun()
    {
        auto active{make_active_with_compile_time_actors<jungles::native::thread, MessagePump>(
            // wrap<Event1>(&Backgrounder::event1_handler),
            [&](Event1&& evt) { event1_handler(std::move(evt)); },
            [&](Event2&& evt) { event2_handler(std::move(evt)); },
            [&](Event3&& evt) { event3_handler(std::move(evt)); })};
    }
    template<typename... Actors>
    using Active_ = Active<jungles::native::thread, MessagePump>;

    // Active_ active{[&](Event1&& evt) { event1_handler(std::move(evt)); },
    //                [&](Event2&& evt) { event2_handler(std::move(evt)); },
    //                [&](Event3&& evt) {
    //                    event3_handler(std::move(evt));
    //                }};

    // void do_one_thing(Event1 evt1)
    // {
    //     active.send(std::move(evt1));
    // }
    //
    // void do_other_thing(Event2 evt2)
    // {
    //     active.send(std::move(evt2));
    // }
    //
    // void do_another_thing(Event3 evt3)
    // {
    //     active.send(std::move(evt3));
    // }
};

TEST_CASE("", "[]") // NOLINT(cert-err58-cpp)
{
    // Actor1 a1;
    // ActiveWithCompileTimeActors<std::thread, MessagePump>
    // ActiveWithCompileTimeActors active
    // {[&](Event
}

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
};

struct Event2
{
};

struct InvalidEvent
{
};

using Thread = jungles::native::thread;

template<typename Message>
using MessagePump = jungles::native::message_pump<Message>;

TEST_CASE("", "[]") // NOLINT(cert-err58-cpp)
{
    auto active{make_active_with_compile_time_actors<Thread, MessagePump>([](Event1) {}, [](Event2) {})};
    active.send(Event1{});
    active.send(Event2{});
    // active.send(InvalidEvent{});
    // Actor1 a1;
    // ActiveWithCompileTimeActors<std::thread, MessagePump>
    // ActiveWithCompileTimeActors active
    // {[&](Event
}

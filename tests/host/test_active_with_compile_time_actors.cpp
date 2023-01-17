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
    int id{0};
};

struct Event2
{
    int id{0};
};

struct Event3
{
};

struct InvalidEvent
{
};

using Thread = jungles::native::thread;

template<typename Message>
using MessagePump = jungles::native::message_pump<Message>;

TEST_CASE("Active runs job in parallel", "[ActiveWithCompileTimeActors]") // NOLINT(cert-err58-cpp)
{
    std::string s{"YOLO"};
    // FIXME: Use rvalue references for parameters
    auto active{make_active_with_compile_time_actors<Thread, MessagePump>(
        [](Event1 e) { std::cout << "Handling Event1: " << e.id << std::endl; },
        [](Event2 e) { std::cout << "Handling Event2: " << e.id << std::endl; },
        [&s](Event3) {
            std::cout << "Handling Event3: " << s << std::endl;
            s = "HAHAHA";
        })};
    active.send(Event1{.id = 10});
    active.send(Event2{.id = 20});
    active.send(Event3{});
}

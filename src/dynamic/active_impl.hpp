/**
 * @file        active.hpp
 * @brief       Active implementation.
 */
#ifndef ACTIVE_IMPL_HPP
#define ACTIVE_IMPL_HPP

#include <functional>

#include "interfaces/active.hpp"
#include "interfaces/message_pump.hpp"
#include "interfaces/thread.hpp"

struct ActiveImpl : Active
{
    using Message = MessagePump::Message;

    explicit ActiveImpl(Thread& thread, MessagePump& message_pump) : thread(thread), message_pump(message_pump)
    {
        thread.start([this]() { this->run(); });
    }

    ~ActiveImpl() override
    {
        message_pump.send([&]() { done = true; });
        thread.join();
    }

    void send(std::function<void(void)> m) override
    {
        message_pump.send(std::move(m));
    }

  private:
    void run()
    {
        while (not done)
        {
            auto msg{message_pump.receive()};
            msg();
        }
    }

    Thread& thread;
    MessagePump& message_pump;
    bool done{false};
};

#endif /* ACTIVE_HPP */

/**
 * @file        message_pump_impl.hpp
 * @brief
 */
#ifndef MESSAGE_PUMP_IMPL_HPP
#define MESSAGE_PUMP_IMPL_HPP

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

#include "interfaces/message_pump.hpp"

struct MessagePumpImpl : MessagePump
{
    void send(Message m) override
    {
        {
            std::lock_guard g{mux};
            queue.push(std::move(m));
        }
        cv.notify_all();
    }

    Message receive() override
    {
        std::unique_lock ul{mux};
        cv.wait(ul, [this]() { return !queue.empty(); });
        return pop();
    }

    std::optional<Message> receive_immediate()
    {
        std::lock_guard g{mux};
        if (queue.empty())
            return std::nullopt;
        return pop();
    }

  private:
    //! Call only if it is certain that the queue is not empty, and the mux must be taken while calling this function.
    Message pop()
    {
        auto r{std::move(queue.front())};
        queue.pop();
        return r;
    }

    std::condition_variable cv;
    std::mutex mux;
    std::queue<Message> queue;
};

#endif /* MESSAGE_PUMP_IMPL_HPP */

/**
 * @file        message_pump.hpp
 * @brief       Message pump interface.
 */
#ifndef MESSAGE_PUMP_HPP
#define MESSAGE_PUMP_HPP

#include <functional>

struct MessagePump
{
    using Message = std::function<void(void)>;

    virtual void send(Message msg) = 0;
    virtual Message receive() = 0;
    virtual ~MessagePump() = default;
};

#endif /* MESSAGE_PUMP_HPP */

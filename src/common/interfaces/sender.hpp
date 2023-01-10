/**
 * @file        sender.hpp
 * @brief       Sender interface.
 */
#ifndef SENDER_HPP
#define SENDER_HPP

template<typename T>
struct Sender
{
    virtual void send_async(T) = 0;
    virtual ~Sender() = default;
};

#endif /* SENDER_HPP */

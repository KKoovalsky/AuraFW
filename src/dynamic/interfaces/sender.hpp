/**
 * @file        sender.hpp
 * @brief       Sender interface.
 */
#ifndef SENDER_HPP
#define SENDER_HPP

template<typename Package>
struct Sender
{
    virtual void send(Package) = 0;
    virtual ~Sender() = default;
};

#endif /* SENDER_HPP */

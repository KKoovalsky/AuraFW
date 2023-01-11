/**
 * @file        listener.hpp
 * @brief       A Listener interface.
 */
#ifndef LISTENER_HPP
#define LISTENER_HPP

template<typename Event>
struct Listener
{
    virtual void update(const Event&) = 0;
    virtual ~Listener() = default;
};

#endif /* LISTENER_HPP */

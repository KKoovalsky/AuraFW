/**
 * @file        notifiable.hpp
 * @brief       Notifiable interface.
 */
#ifndef NOTIFIABLE_HPP
#define NOTIFIABLE_HPP

template<typename Event>
struct Notifiable
{
    virtual void notify(const Event&) = 0;
    virtual ~Notifiable() = default;
};

#endif /* NOTIFIABLE_HPP */

/**
 * @file        sender_impl.hpp
 * @brief       Sender implementation.
 */
#ifndef SENDER_IMPL_HPP
#define SENDER_IMPL_HPP

#include "interfaces/sender.hpp"

template<typename T>
class SenderImpl : Sender<T>
{
  public:
    explicit SenderImpl();

    void send(T) override
    {
        // TODO: A way to dispatch this to an active object
        // It might be:
        // 1. notify(SendRequest(T))
        // 2. direct use of an active object
    }

  private:
};

#endif /* SENDER_IMPL_HPP */

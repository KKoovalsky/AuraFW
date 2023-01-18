/**
 * @file        native_timer.hpp
 * @brief       A timer which can only run on a host machine.
 */
#ifndef NATIVE_TIMER_HPP
#define NATIVE_TIMER_HPP

#include <chrono>

#include <concurrencpp/concurrencpp.h>

#include "interfaces/notifiable.hpp"

template<typename Event>
struct NativeTimer
{
    explicit NativeTimer(Notifiable<Event>& notifiable,
                         concurrencpp::runtime& runtime,
                         std::chrono::milliseconds interval) :
        notifiable{notifiable},
        timer{runtime.timer_queue()->make_timer(
            interval, interval, runtime.thread_pool_executor(), [this]() { this->notifiable.notify(Event{}); })}
    {
    }

  private:
    Notifiable<Event>& notifiable;
    concurrencpp::timer timer;
};

#endif /* NATIVE_TIMER_HPP */

/**
 * @file        native_timer.hpp
 * @brief       A timer which can only run on a host machine.
 */
#ifndef NATIVE_TIMER_HPP
#define NATIVE_TIMER_HPP

#include <chrono>
#include <concurrencpp/runtime/runtime.h>

#include "concurrencpp/concurrencpp.h"

template<typename Notifier>
struct NativeTimer
{
    explicit NativeTimer(Notifier& notifier, concurrencpp::runtime& runtime, std::chrono::milliseconds interval) :
        notifier{notifier},
        timer{runtime.timer_queue()->make_timer(
            interval, interval, runtime.thread_pool_executor(), [this]() { this->notifier.notify(); })}
    {
    }

  private:
    Notifier& notifier;
    concurrencpp::timer timer;
};

#endif /* NATIVE_TIMER_HPP */

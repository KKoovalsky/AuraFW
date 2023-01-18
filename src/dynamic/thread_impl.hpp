/**
 * @file        thread_impl.hpp
 * @brief       Thread implementation.
 */
#ifndef THREAD_IMPL_HPP
#define THREAD_IMPL_HPP

#include <functional>
#include <thread>

#include "interfaces/thread.hpp"

struct ThreadImpl : Thread
{
    void start(std::function<void(void)> f) override
    {
        thread = std::thread{std::move(f)};
    }

    void join() override
    {
        thread.join();
    }

  private:
    std::thread thread;
};

#endif /* THREAD_IMPL_HPP */

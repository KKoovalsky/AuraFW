/**
 * @file        thread.hpp
 * @brief       thread.hpperface.
 */
#ifndef THREAD_HPP
#define THREAD_HPP

#include <functional>

struct Thread
{
    virtual void start(std::function<void(void)>) = 0;
    virtual void join() = 0;
    virtual ~Thread() = default;
};

#endif /* THREAD_HPP */

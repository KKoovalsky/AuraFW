/**
 * @file        active.hpp
 * @brief       Active interface.
 */
#ifndef ACTIVE_HPP
#define ACTIVE_HPP

#include <functional>

struct Active
{
    virtual void send(std::function<void(void)>) = 0;
    virtual ~Active() = default;
};

#endif /* ACTIVE_HPP */

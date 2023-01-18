/**
 * @file        observer.hpp
 * @brief       Observer interface.
 */
#ifndef OBSERVER_HPP
#define OBSERVER_HPP

template<typename Event>
struct Observer
{
    virtual void update(const Event&) = 0;
    virtual ~Observer() = default;
};

#endif /* OBSERVER_HPP */

/**
 * @file        signal.hpp
 * @brief       Signal implementation.
 */
#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <vector>

#include "interfaces/notifiable.hpp"
#include "interfaces/observer.hpp"

template<typename Event>
class Signal : public Notifiable<Event>
{
  public:
    explicit Signal()
    {
        observers.reserve(4);
    }

    void notify(const Event& evt) override
    {
        for (auto obs : observers)
            obs->update(evt);
    }

    void add(Observer<Event>* observer)
    {
        observers.push_back(observer);
    }

  private:
    std::vector<Observer<Event>*> observers;
};

#endif /* SIGNAL_HPP */

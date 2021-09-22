/**
 * @file        periodical_timer.hpp
 * @brief       Periodical Timer interface.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef PERIODICAL_TIMER_HPP
#define PERIODICAL_TIMER_HPP

#include <chrono>
#include <functional>

class PeriodicalTimer
{
  public:
    using PeriodicalCallback = std::function<void(void)>;

    virtual void start(std::chrono::seconds period, PeriodicalCallback) = 0;
    virtual ~PeriodicalTimer() = default;

  private:
};

#endif /* PERIODICAL_TIMER_HPP */

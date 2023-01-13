/**
 * @file        collector_using_static_polymorphizm.hpp
 * @brief       Collects measurements and sends them on sending interval.
 */
#ifndef COLLECTOR_USING_STATIC_POLYMORPHIZM_HPP
#define COLLECTOR_USING_STATIC_POLYMORPHIZM_HPP

#include <utility>

#include "events.hpp"

template<typename Measurement, typename Package, typename Store, typename Measurer, typename Sender>
class CollectorWhichUsesStaticPolymporphizm
{
  public:
    constexpr explicit CollectorWhichUsesStaticPolymporphizm(Store& store, Measurer& measurer, Sender& sender) :
        store{store}, measurer{measurer}, sender{sender}
    {
    }

    void update(const Event::MeasurementInterval&) const
    {
        auto measurement{measurer.measure()};
        store.store(std::move(measurement));
    }

    void update(const Event::SendingInterval&) const
    {
        auto package{store.package_and_clear()};
        sender.send(std::move(package));
    }

  private:
    Store& store;
    Measurer& measurer;
    Sender& sender;
};

#endif /* COLLECTOR_USING_STATIC_POLYMORPHIZM_HPP */

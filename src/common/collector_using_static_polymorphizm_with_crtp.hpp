/**
 * @file        collector_using_static_polymorphizm_with_crtp.hpp
 * @brief       Collects measurements and sends them on sending interval.
 */
#ifndef COLLECTOR_USING_STATIC_POLYMORPHIZM_AND_CRTP_HPP
#define COLLECTOR_USING_STATIC_POLYMORPHIZM_AND_CRTP_HPP

#include <utility>

#include "events.hpp"

template<typename Measurement, typename Package, typename CRTP>
struct Store
{
    void store_(Measurement m)
    {
        get_underlying().store(std::move(m));
    }

    Package package_and_clear_()
    {
        return get_underlying().package_and_clear();
    }

    CRTP& get_underlying()
    {
        return static_cast<CRTP&>(*this);
    }
};

template<typename Measurement, typename CRTP>
struct Measurer
{
    Measurement measure_()
    {
        return get_underlying().measure();
    }

    CRTP& get_underlying()
    {
        return static_cast<CRTP&>(*this);
    }
};

template<typename Package, typename CRTP>
struct Sender
{
    void send_(Package p)
    {
        get_underlying().send(std::move(p));
    }

    CRTP& get_underlying()
    {
        return static_cast<CRTP&>(*this);
    }
};

template<typename Measurement, typename Package, typename StoreImpl, typename MeasurerImpl, typename SenderImpl>
class CollectorWhichUsesStaticPolymporphizmWithCrtp
{
  public:
    using Store_ = Store<Measurement, Package, StoreImpl>;
    using Measurer_ = Measurer<Measurement, MeasurerImpl>;
    using Sender_ = Sender<Package, SenderImpl>;

    constexpr explicit CollectorWhichUsesStaticPolymporphizmWithCrtp(Store_& store,
                                                                     Measurer_& measurer,
                                                                     Sender_& sender) :
        store{store}, measurer{measurer}, sender{sender}
    {
    }

    void update(const Event::MeasurementInterval&) const
    {
        auto measurement{measurer.measure_()};
        store.store_(std::move(measurement));
    }

    void update(const Event::SendingInterval&) const
    {
        auto package{store.package_and_clear_()};
        sender.send_(std::move(package));
    }

  private:
    Store_& store;
    Measurer_& measurer;
    Sender_& sender;
};

#endif /* COLLECTOR_USING_STATIC_POLYMORPHIZM_HPP */

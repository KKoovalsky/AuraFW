/**
 * @file        measurement_scheduler.hpp
 * @brief       Schedules measurement collecting and publishing.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef MEASUREMENT_SCHEDULER_HPP
#define MEASUREMENT_SCHEDULER_HPP

#include "measurer.hpp"
#include "periodical_timer.hpp"
#include "publisher.hpp"

#include <chrono>
#include <cmath>
#include <exception>
#include <string>

namespace detail
{
struct UnsignedInteger
{
    constexpr explicit UnsignedInteger(unsigned value) : value{value}
    {
    }

    const unsigned value;
};
} // namespace detail

struct MeasurementIntervalInSeconds : detail::UnsignedInteger
{
    constexpr explicit MeasurementIntervalInSeconds(unsigned value) : detail::UnsignedInteger{value}
    {
    }
};

struct PublishingIntervalInSeconds : detail::UnsignedInteger
{
    constexpr explicit PublishingIntervalInSeconds(unsigned value) : detail::UnsignedInteger{value}
    {
    }
};

template<typename Measurement>
class MeasurementScheduler
{
  public:
    using MeasurerBridge = Measurer<Measurement>;
    using PublisherBridge = Publisher<Measurement>;

    explicit MeasurementScheduler(MeasurementIntervalInSeconds measurement_interval,
                                  PublishingIntervalInSeconds publishing_interval,
                                  MeasurerBridge& measurer,
                                  PublisherBridge& publisher,
                                  PeriodicalTimer& measurement_interval_timer,
                                  PeriodicalTimer& publishing_interval_timer) :
        measurement_interval{std::chrono::seconds(measurement_interval.value)},
        publishing_interval{std::chrono::seconds(publishing_interval.value)},
        measurer{measurer},
        publisher{publisher},
        measurement_interval_timer{measurement_interval_timer},
        publishing_interval_timer{publishing_interval_timer}
    {
        throw_if_measurement_interval_higher_than_publishing_interval();
        reserve_space_for_measurements();

        measurement_interval_timer.start(this->measurement_interval, [this]() {
            // TODO: mutex here
            auto measurement{this->measurer.measure()};
            collected_measurements.emplace_back(std::move(measurement));
        });

        publishing_interval_timer.start(this->publishing_interval, [this]() {
            this->publisher.publish(std::move(collected_measurements));
            reserve_space_for_measurements();
        });
    }

  private:
    struct Error : std::exception
    {
        Error(std::string message) : message{std::move(message)}
        {
        }

        virtual const char* what() const noexcept
        {
            return message.c_str();
        }

        std::string message;
    };

    void throw_if_measurement_interval_higher_than_publishing_interval()
    {
        if (measurement_interval > publishing_interval)
            throw Error{"Measurement Interval cannot be higher than Publishing Interval"};
    }

    void reserve_space_for_measurements()
    {
        auto publishing_interval_{static_cast<float>(publishing_interval.count())};
        auto measurement_interval_{static_cast<float>(measurement_interval.count())};
        auto number_of_measurements_in_package{
            static_cast<unsigned>(std::ceil(publishing_interval_ / measurement_interval_))};
        collected_measurements.reserve(number_of_measurements_in_package);
    }

    std::chrono::seconds measurement_interval;
    std::chrono::seconds publishing_interval;
    MeasurerBridge& measurer;
    PublisherBridge& publisher;
    PeriodicalTimer& measurement_interval_timer;
    PeriodicalTimer& publishing_interval_timer;

    std::vector<Measurement> collected_measurements;
};

#endif /* MEASUREMENT_SCHEDULER_HPP */

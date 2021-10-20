/**
 * @file        measurement_scheduler.hpp
 * @brief       Schedules measurement collecting and publishing.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef MEASUREMENT_SCHEDULER_HPP
#define MEASUREMENT_SCHEDULER_HPP

#include "loggable.hpp"
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

template<typename Measurement, typename Logger>
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
                                  PeriodicalTimer& publishing_interval_timer,
                                  Loggable auto& logger) :
        measurement_interval{std::chrono::seconds(measurement_interval.value)},
        publishing_interval{std::chrono::seconds(publishing_interval.value)},
        measurer{measurer},
        publisher{publisher},
        measurement_interval_timer{measurement_interval_timer},
        publishing_interval_timer{publishing_interval_timer},
        logger{logger}
    {
        throw_if_measurement_interval_higher_than_publishing_interval();
        reserve_space_for_measurements();

        this->measurement_interval_timer.start(this->measurement_interval, [this]() {
            try
            {
                // TODO: mutex here
                auto measurement{this->measurer.measure()};
                collected_measurements.emplace_back(std::move(measurement));
            } catch (const typename MeasurerBridge::Error& error)
            {
                this->logger.log(LogLevel::error) << "Measurer: " << error.what();
            }
        });

        this->publishing_interval_timer.start(this->publishing_interval, [this]() {
            try
            {
                // TODO: mutex here
                this->publisher.publish(std::move(collected_measurements));
                reserve_space_for_measurements();
            } catch (const typename PublisherBridge::Error& error)
            {
                this->logger.log(LogLevel::error) << "Publisher: " << error.what();
            }
        });
    }

  private:
    struct Error : std::exception
    {
        explicit Error(std::string message) : message{std::move(message)}
        {
        }

        [[nodiscard]] const char* what() const noexcept override
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
    Logger& logger;

    std::vector<Measurement> collected_measurements;
};

#endif /* MEASUREMENT_SCHEDULER_HPP */

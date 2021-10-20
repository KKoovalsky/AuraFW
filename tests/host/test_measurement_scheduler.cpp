/**
 * @file        test_measurement_scheduler.cpp
 * @brief       Tests the MeasurementScheduler module.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include <chrono>
#include <list>
#include <vector>

#include "catch2/matchers/catch_matchers_string.hpp"
#include "measurement_scheduler.hpp"

struct MeasurementMock
{
    unsigned id;
};

struct MeasurerMock : Measurer<MeasurementMock>
{
    MeasurementMock measure() override
    {
        measured_times++;
        return measurement;
    }

    unsigned measured_times{0};
    MeasurementMock measurement{};
};

struct PublisherMock : Publisher<MeasurementMock>
{
    void publish(MultipleMeasurements measurements) override
    {
        if (error.has_value())
        {
            throw *error;
        }
        published_times++;
        published_measurements.push_back(measurements);
    }

    struct Error : Publisher<MeasurementMock>::Error
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

    unsigned published_times{0};
    std::list<MultipleMeasurements> published_measurements;
    std::optional<Error> error;
};

struct PeriodicalTimerMock : PeriodicalTimer
{
    void start(std::chrono::seconds timer_period, PeriodicalCallback callback) override
    {
        this->period = timer_period;
        periodical_callback = callback;
    }

    PeriodicalCallback periodical_callback;
    std::chrono::seconds period{};
};

struct LoggerMock
{
    LoggerMock& log(LogLevel log_level)
    {
        previous_log_level = log_level;
        return *this;
    }

    template<class T>
    LoggerMock& operator<<(T value)
    {
        if (previous_log_level == LogLevel::error)
        {
            error_messages.emplace_back(value);
        }
        return *this;
    }

    std::vector<std::string> error_messages;
    LogLevel previous_log_level{LogLevel::info};
};

bool operator!=(const MeasurementMock& l, const MeasurementMock& r)
{
    return l.id != r.id;
}

TEST_CASE("Measurements are scheduled for collecting and publishing. According to requirements: OP",
          "[measurement_scheduler]")
{
    using MeasurementSchedulerUnderTest = MeasurementScheduler<MeasurementMock, LoggerMock>;
    MeasurerMock measurer;
    PublisherMock publisher;
    PeriodicalTimerMock measuring_interval_timer;
    PeriodicalTimerMock publishing_interval_timer;
    LoggerMock logger;

    auto make_measurement_scheduler{
        [&](MeasurementIntervalInSeconds measurement_interval, PublishingIntervalInSeconds publishing_interval) {
            return MeasurementSchedulerUnderTest{measurement_interval,
                                                 publishing_interval,
                                                 measurer,
                                                 publisher,
                                                 measuring_interval_timer,
                                                 publishing_interval_timer,
                                                 logger};
        }};

    auto make_default_measurement_scheduler{[&]() {
        return MeasurementSchedulerUnderTest{MeasurementIntervalInSeconds{1},
                                             PublishingIntervalInSeconds{2},
                                             measurer,
                                             publisher,
                                             measuring_interval_timer,
                                             publishing_interval_timer,
                                             logger};
    }};

    SECTION("No measurement is collected before measurement interval has passed")
    {
        [[maybe_unused]] auto scheduler{
            make_measurement_scheduler(MeasurementIntervalInSeconds(1), PublishingIntervalInSeconds(2))};
        REQUIRE(measurer.measured_times == 0);
    }

    SECTION("On each Measurement Interval Measurements are collected")
    {
        [[maybe_unused]] auto scheduler{
            make_measurement_scheduler(MeasurementIntervalInSeconds{1}, PublishingIntervalInSeconds{2})};
        measuring_interval_timer.periodical_callback();
        measuring_interval_timer.periodical_callback();
        REQUIRE(measurer.measured_times == 2);
    }

    SECTION("No measurement is published before publishing interval has passed")
    {
        [[maybe_unused]] auto scheduler{
            make_measurement_scheduler(MeasurementIntervalInSeconds{1}, PublishingIntervalInSeconds{2})};
        REQUIRE(publisher.published_times == 0);
    }

    SECTION("On each Publishing Interval Measurements are published")
    {
        [[maybe_unused]] auto scheduler{
            make_measurement_scheduler(MeasurementIntervalInSeconds{1}, PublishingIntervalInSeconds{2})};
        publishing_interval_timer.periodical_callback();
        publishing_interval_timer.periodical_callback();
        REQUIRE(publisher.published_times == 2);
    }

    SECTION("All the collected Measurements are published ...")
    {
        [[maybe_unused]] auto scheduler{
            make_measurement_scheduler(MeasurementIntervalInSeconds{1}, PublishingIntervalInSeconds{2})};

        SECTION(" ... on first Publishing Interval")
        {
            measurer.measurement = MeasurementMock{.id = 11};
            measuring_interval_timer.periodical_callback();
            measurer.measurement = MeasurementMock{.id = 13};
            measuring_interval_timer.periodical_callback();

            publishing_interval_timer.periodical_callback();

            REQUIRE_THAT(publisher.published_measurements.front(),
                         Catch::Matchers::Equals(std::vector{MeasurementMock{.id = 11}, MeasurementMock{.id = 13}}));
            publisher.published_measurements.pop_front();

            SECTION(" ... on second Publishing Interval")
            {
                measurer.measurement = MeasurementMock{.id = 22};
                measuring_interval_timer.periodical_callback();
                measurer.measurement = MeasurementMock{.id = 26};
                measuring_interval_timer.periodical_callback();

                publishing_interval_timer.periodical_callback();

                REQUIRE_THAT(
                    publisher.published_measurements.front(),
                    Catch::Matchers::Equals(std::vector{MeasurementMock{.id = 22}, MeasurementMock{.id = 26}}));
            }
        }
    }

    SECTION("Measurement Interval higher than Publishing Interval is an error")
    {
        auto create_measurement_scheduler_with_wrong_intervals{[&]() {
            make_measurement_scheduler(MeasurementIntervalInSeconds{3}, PublishingIntervalInSeconds{2});
        }};
        REQUIRE_THROWS_WITH(create_measurement_scheduler_with_wrong_intervals(),
                            "Measurement Interval cannot be higher than Publishing Interval");
    }

    SECTION("Measurement Interval equal to Publishing Interval is not an error")
    {
        auto create_measurement_scheduler_with_intervals_equal{[&]() {
            make_measurement_scheduler(MeasurementIntervalInSeconds{100}, PublishingIntervalInSeconds{100});
        }};
        REQUIRE_NOTHROW(create_measurement_scheduler_with_intervals_equal());
    }

    SECTION("No measurements to publish is not an error")
    {
        auto call_publishing_interval_before_measurement_interval{[&]() {
            [[maybe_unused]] auto scheduler{
                make_measurement_scheduler(MeasurementIntervalInSeconds{3}, PublishingIntervalInSeconds{7})};
            publishing_interval_timer.periodical_callback();
        }};
        REQUIRE_NOTHROW(call_publishing_interval_before_measurement_interval());
    }

    SECTION("Proper timeout is set for Measurement Interval timer")
    {
        [[maybe_unused]] auto scheduler{
            make_measurement_scheduler(MeasurementIntervalInSeconds{30}, PublishingIntervalInSeconds{70})};
        REQUIRE(measuring_interval_timer.period == std::chrono::seconds(30));
    }

    SECTION("Proper timeout is set for Publishing Interval timer")
    {
        [[maybe_unused]] auto scheduler{
            make_measurement_scheduler(MeasurementIntervalInSeconds{30}, PublishingIntervalInSeconds{70})};
        REQUIRE(publishing_interval_timer.period == std::chrono::seconds(70));
    }

    SECTION("Error is logged on Publisher error")
    {
        auto scheduler{make_default_measurement_scheduler()};
        publisher.error = PublisherMock::Error{"just testing!"};
        publishing_interval_timer.periodical_callback();
        REQUIRE_THAT(logger.error_messages,
                     Catch::Matchers::Equals(std::vector<std::string>{"Publisher: ", "just testing!"}));
    }

    SECTION("Error is logged on Measurer error")
    {
    }
}
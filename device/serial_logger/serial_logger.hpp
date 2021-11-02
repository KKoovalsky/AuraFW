/**
 * @file        serial_logger.hpp
 * @brief       Defines Serial Logger which implements Loggable for the device.
 * @author      Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef SERIAL_LOGGER_HPP
#define SERIAL_LOGGER_HPP

#include <string>

#include "loggable.hpp"

class SerialLogger
{
  private:
    class SerialLoggerProxy
    {
      public:
        explicit SerialLoggerProxy(LogLevel log_level);
        ~SerialLoggerProxy();

        SerialLoggerProxy& operator<<(std::string);

        static inline constexpr const char* log_level_to_string(LogLevel log_level)
        {
            switch (log_level)
            {
            case LogLevel::info:
                return "INFO: ";
            case LogLevel::warning:
                return "WARNING: ";
            case LogLevel::error:
                return "ERROR: ";
            }
        }
    };

  public:
    explicit SerialLogger();
    ~SerialLogger();
    SerialLoggerProxy log(LogLevel log_level) const;

  private:
};

#endif /* SERIAL_LOGGER_HPP */

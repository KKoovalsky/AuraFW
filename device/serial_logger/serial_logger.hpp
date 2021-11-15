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
        SerialLoggerProxy& operator<<(char);
        SerialLoggerProxy& operator<<(unsigned char);
        SerialLoggerProxy& operator<<(float);
        SerialLoggerProxy& operator<<(unsigned);
        SerialLoggerProxy& operator<<(int);

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

      private:
        void write_bytes_over_uart(unsigned char* data, unsigned length);
        void write_bytes_over_uart(char* data, unsigned length);
    };

  public:
    explicit SerialLogger();
    ~SerialLogger();
    [[nodiscard]] SerialLoggerProxy log(LogLevel log_level) const;

  private:
    static unsigned instance_count;
};

#endif /* SERIAL_LOGGER_HPP */

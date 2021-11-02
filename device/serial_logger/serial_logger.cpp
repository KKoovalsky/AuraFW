/**
 * @file    serial_logger.cpp
 * @brief   Implements the Serial Logger.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "serial_logger.hpp"
#include "usart.h"

SerialLogger::SerialLoggerProxy::SerialLoggerProxy(LogLevel log_level)
{
    *this << log_level_to_string(log_level);
}

SerialLogger::SerialLoggerProxy& SerialLogger::SerialLoggerProxy::operator<<(std::string s)
{
    uint32_t timeout_ms{300};
    HAL_UART_Transmit(&hlpuart1, reinterpret_cast<uint8_t*>(s.data()), s.length(), timeout_ms);
    return *this;
}

SerialLogger::SerialLoggerProxy::~SerialLoggerProxy()
{
    *this << "\r\n";
}

SerialLogger::SerialLoggerProxy SerialLogger::log(LogLevel log_level) const
{
    return SerialLoggerProxy{log_level};
}

SerialLogger::SerialLogger()
{
    MX_LPUART1_UART_Init();
}

SerialLogger::~SerialLogger()
{
    HAL_UART_DeInit(&hlpuart1);
}

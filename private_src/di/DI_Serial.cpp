#include <bsp-interface/di/serial.h>
#include <Serial.h>

/// @brief 主串口。
/// @return
bsp::serial::ISerial &bsp::di::serial::Serial()
{
    return bsp::Serial::Instance();
}

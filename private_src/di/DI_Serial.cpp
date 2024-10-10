#include <bsp-interface/di/serial.h>
#include <Serial.h>

/// @brief 主串口。
/// @return
bsp::ISerial &DI_Serial()
{
    return bsp::Serial::Instance();
}

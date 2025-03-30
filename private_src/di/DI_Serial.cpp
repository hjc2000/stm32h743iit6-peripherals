#include "base/peripheral/ISerial.h"
#include <Serial.h>

base::serial::ISerial &base::serial::MainSerial()
{
	return bsp::Serial::Instance();
}

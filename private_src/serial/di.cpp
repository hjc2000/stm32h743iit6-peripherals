#include "base/container/Dictionary.h"
#include "serial_handle.h"

base::serial::ISerial &base::serial::MainSerial()
{
	return base::serial::serial_handle::Instance();
}

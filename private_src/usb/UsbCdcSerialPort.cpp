#include "UsbCdcSerialPort.h" // IWYU pragma: keep
#include "base/SingletonProvider.h"

namespace
{
	base::SingletonProvider<bsp::UsbCdcSerialPort> _provider;
}

bsp::UsbCdcSerialPort &bsp::usb_cdc_serial_port()
{
	return _provider.Instance();
}

#include "UsbCdcSerialPort.h" // IWYU pragma: keep
#include "base/SingletonProvider.h"

namespace
{
	base::SingletonProvider<base::Slot<bsp::UsbCdcSerialPort>> _provider;
}

base::Slot<bsp::UsbCdcSerialPort> &bsp::usb_cdc_serial_port_slot()
{
	return _provider.Instance();
}

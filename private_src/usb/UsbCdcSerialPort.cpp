#include "UsbCdcSerialPort.h" // IWYU pragma: keep
#include "base/GlobalObjectProvider.h"

namespace
{
	base::GlobalObjectProvider<base::Slot<bsp::UsbCdcSerialPort>> _provider;
}

base::Slot<bsp::UsbCdcSerialPort> &bsp::usb_cdc_serial_port_slot()
{
	return _provider.Instance();
}

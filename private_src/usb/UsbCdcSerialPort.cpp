#include "UsbCdcSerialPort.h" // IWYU pragma: keep
#include "base/SingletonProvider.h"

bsp::UsbCdcSerialPort::UsbCdcSerialPort(std::shared_ptr<base::usb::fs_device_pcd::UsbFsDevicePcd> const &pcd)
{
	_pcd = pcd;

	/* Init Device Library, add supported class and start the library. */
	if (USBD_Init(&_handle_context._handle, &FS_Desc, 0) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	if (USBD_RegisterClass(&_handle_context._handle, &USBD_CDC) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	if (USBD_CDC_RegisterInterface(&_handle_context._handle, &USBD_Interface_fops_FS) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	if (USBD_Start(&_handle_context._handle) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	HAL_PWREx_EnableUSBVoltageDetector();
}

namespace
{
	base::SingletonProvider<base::Slot<bsp::UsbCdcSerialPort>> _provider;
}

base::Slot<bsp::UsbCdcSerialPort> &bsp::usb_cdc_serial_port_slot()
{
	return _provider.Instance();
}

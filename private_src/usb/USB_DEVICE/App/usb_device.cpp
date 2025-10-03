#include "usb_device.h"
#include "base/embedded/usb/fs-device-pcd/UsbFsDevicePcd.h"
#include "UsbCdcSerialPort.h"
#include <memory>

/**
 * Init USB device Library, add supported class and start the library
 * @retval None
 */
void MX_USB_DEVICE_Init(void)
{
	std::shared_ptr<base::usb::fs_device_pcd::UsbFsDevicePcd> pcd = base::usb::fs_device_pcd::usb_fs_pcd_slot()[0];
	std::shared_ptr<bsp::UsbCdcSerialPort> port{new bsp::UsbCdcSerialPort{pcd}};
	bsp::usb_cdc_serial_port_slot().Add(port);
}

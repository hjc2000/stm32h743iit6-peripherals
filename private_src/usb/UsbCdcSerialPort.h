#pragma once
#include "base/embedded/Slot.h"
#include "base/embedded/usb/fs-device-pcd/usb_fs_device_pcd_handle.h"
#include "base/embedded/usb/fs-device-pcd/UsbFsDevicePcd.h"
#include "base/string/define.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "usb_device.h"
#include "usb_fs_device_pcd_handle.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_core.h"
#include "usbd_def.h"
#include "usbd_desc.h"
#include <cstdint>
#include <memory>
#include <stdexcept>

namespace bsp
{
	class UsbCdcSerialPort
	{
	private:
		class handle_context
		{
		public:
			handle_context(UsbCdcSerialPort *self)
				: _self{self}
			{
			}

			USBD_HandleTypeDef _handle{};
			UsbCdcSerialPort *_self{};
		};

		handle_context _handle_context{this};
		std::shared_ptr<base::usb::fs_device_pcd::UsbFsDevicePcd> _pcd;

	public:
		UsbCdcSerialPort(std::shared_ptr<base::usb::fs_device_pcd::UsbFsDevicePcd> const &pcd);

		USBD_HandleTypeDef &UsbdHandle()
		{
			return _handle_context._handle;
		}
	};

	base::Slot<bsp::UsbCdcSerialPort> &usb_cdc_serial_port_slot();

} // namespace bsp

#pragma once
#include "base/embedded/Slot.h"
#include "base/embedded/usb/fs-device-pcd/usb_fs_pcd_handle.h"
#include "base/embedded/usb/fs-device-pcd/UsbFsPcd.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "usb_fs_pcd_handle.h"
#include "usbd_def.h"
#include <cstdint>
#include <memory>

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
		std::shared_ptr<base::usb::fs_device_pcd::UsbFsPcd> _pcd;

	public:
		UsbCdcSerialPort(std::shared_ptr<base::usb::fs_device_pcd::UsbFsPcd> const &pcd)
		{
			_pcd = pcd;
		}

		USBD_HandleTypeDef &UsbdHandle()
		{
			return _handle_context._handle;
		}
	};

	base::Slot<bsp::UsbCdcSerialPort> &usb_cdc_serial_port_slot();

} // namespace bsp

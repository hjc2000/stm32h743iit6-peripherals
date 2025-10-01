#pragma once
#include "base/embedded/usb/fs-device-pcd/usb_fs_pcd_handle.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "usb_fs_pcd_handle.h"
#include "usbd_def.h"
#include <cstdint>

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

	public:
		USBD_HandleTypeDef &UsbdHandle()
		{
			return _handle_context._handle;
		}
	};

	bsp::UsbCdcSerialPort &usb_cdc_serial_port();

} // namespace bsp

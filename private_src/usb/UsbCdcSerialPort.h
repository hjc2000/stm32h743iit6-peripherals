#pragma once
#include "base/embedded/usb/usb_fs_pcd_handle.h"
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
		class usbd_handle_context
		{
		public:
			usbd_handle_context(UsbCdcSerialPort *self)
				: _self{self}
			{
			}

			inline static USBD_HandleTypeDef _handle{};
			UsbCdcSerialPort *_self{};
		};

	public:

		static USBD_HandleTypeDef &UsbdHandle()
		{
			return usbd_handle_context::_handle;
		}
	};

} // namespace bsp

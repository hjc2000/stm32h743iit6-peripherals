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

uint8_t *USBD_FS_DeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *USBD_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

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

		USBD_DescriptorsTypeDef FS_Desc = {
			USBD_FS_DeviceDescriptor,
			USBD_FS_LangIDStrDescriptor,
			USBD_FS_ManufacturerStrDescriptor,
			USBD_FS_ProductStrDescriptor,
			USBD_FS_SerialStrDescriptor,
			USBD_FS_ConfigStrDescriptor,
			USBD_FS_InterfaceStrDescriptor,
		};

	public:
		UsbCdcSerialPort(std::shared_ptr<base::usb::fs_device_pcd::UsbFsDevicePcd> const &pcd)
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

		USBD_HandleTypeDef &UsbdHandle()
		{
			return _handle_context._handle;
		}
	};

	base::Slot<bsp::UsbCdcSerialPort> &usb_cdc_serial_port_slot();

} // namespace bsp

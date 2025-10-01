/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usb_device.c
 * @version        : v1.0_Cube
 * @brief          : This file implements the USB Device
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "base/embedded/usb/fs-device-pcd/UsbFsPcd.h"
#include "base/string/define.h"
#include "UsbCdcSerialPort.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include <memory>
#include <stdexcept>

/**
 * Init USB device Library, add supported class and start the library
 * @retval None
 */
void MX_USB_DEVICE_Init(void)
{
	std::shared_ptr<base::usb::fs_device_pcd::UsbFsPcd> pcd = base::usb::fs_device_pcd::usb_fs_pcd_slot()[0];
	std::shared_ptr<bsp::UsbCdcSerialPort> port{new bsp::UsbCdcSerialPort{pcd}};
	bsp::usb_cdc_serial_port_slot().Add(port);

	/* Init Device Library, add supported class and start the library. */
	if (USBD_Init(&bsp::usb_cdc_serial_port_slot()[0]->UsbdHandle(), &FS_Desc, 0) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	if (USBD_RegisterClass(&bsp::usb_cdc_serial_port_slot()[0]->UsbdHandle(), &USBD_CDC) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	if (USBD_CDC_RegisterInterface(&bsp::usb_cdc_serial_port_slot()[0]->UsbdHandle(), &USBD_Interface_fops_FS) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	if (USBD_Start(&bsp::usb_cdc_serial_port_slot()[0]->UsbdHandle()) != USBD_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	HAL_PWREx_EnableUSBVoltageDetector();
}

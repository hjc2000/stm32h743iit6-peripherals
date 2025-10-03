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
#include "base/embedded/usb/fs-device-pcd/UsbFsDevicePcd.h"
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
	std::shared_ptr<base::usb::fs_device_pcd::UsbFsDevicePcd> pcd = base::usb::fs_device_pcd::usb_fs_pcd_slot()[0];
	std::shared_ptr<bsp::UsbCdcSerialPort> port{new bsp::UsbCdcSerialPort{pcd}};
	bsp::usb_cdc_serial_port_slot().Add(port);
}

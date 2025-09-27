/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : Target/usbd_conf.c
 * @version        : v1.0_Cube
 * @brief          : This file implements the board support package for the USB device library
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
#include "base/embedded/usb/UsbFsPcd.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_pcd.h"
#include "UsbCdcSerialPort.h"
#include "usbd_cdc.h"
#include "usbd_core.h"
#include "usbd_def.h"
#include "UsbFsPcd.h"
#include <cstdint>
#include <memory>

USBD_StatusTypeDef USBD_Get_USB_Status(HAL_StatusTypeDef hal_status);

/**
 * @brief  Data In stage callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint number
 * @retval None
 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#else
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
{
	USBD_LL_DataInStage((USBD_HandleTypeDef *)hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

/**
 * @brief  ISOOUTIncomplete callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint number
 * @retval None
 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#else
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
{
	USBD_LL_IsoOUTIncomplete((USBD_HandleTypeDef *)hpcd->pData, epnum);
}

/**
 * @brief  ISOINIncomplete callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint number
 * @retval None
 */
#if (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
static void PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#else
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
{
	USBD_LL_IsoINIncomplete((USBD_HandleTypeDef *)hpcd->pData, epnum);
}

/*******************************************************************************
					   LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

/**
 * @brief  Initializes the low level portion of the device driver.
 * @param  pdev: Device handle
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev)
{
	bsp::UsbFsPcd::HalPcdHandle().pData = pdev;
	pdev->pData = &bsp::UsbFsPcd::HalPcdHandle();

	std::shared_ptr<base::usb::fs_pcd::UsbFsPcd> pcd = base::usb::fs_pcd::usb_fs_pcd_slot()[0];

	pcd->SetSofCallback([]()
						{
							USBD_LL_SOF(&bsp::UsbCdcSerialPort::UsbdHandle());
						});

	pcd->SetSetupStageCallback([](base::usb::fs_pcd::SetupStageCallbackArgs const &args)
							   {
								   USBD_LL_SetupStage(&bsp::UsbCdcSerialPort::UsbdHandle(), const_cast<uint8_t *>(args.Span().Buffer()));
							   });

	pcd->SetResetCallback([]()
						  {
							  USBD_LL_SetSpeed(&bsp::UsbCdcSerialPort::UsbdHandle(), USBD_SPEED_FULL);
							  USBD_LL_Reset(&bsp::UsbCdcSerialPort::UsbdHandle());
						  });

	pcd->SetSuspendCallback([]()
							{
								/* Inform USB library that core enters in suspend Mode. */
								USBD_LL_Suspend(&bsp::UsbCdcSerialPort::UsbdHandle());

								std::shared_ptr<base::usb::fs_pcd::UsbFsPcd> pcd = base::usb::fs_pcd::usb_fs_pcd_slot()[0];
								pcd->Suspend();
							});

	pcd->SetResumeCallback([]()
						   {
							   std::shared_ptr<base::usb::fs_pcd::UsbFsPcd> pcd = base::usb::fs_pcd::usb_fs_pcd_slot()[0];
							   pcd->Resume();

							   USBD_LL_Resume(&bsp::UsbCdcSerialPort::UsbdHandle());
						   });

	pcd->SetConnectCallback([]()
							{
								USBD_LL_DevConnected(&bsp::UsbCdcSerialPort::UsbdHandle());
							});

	pcd->SetDisconnectCallback([]()
							   {
								   USBD_LL_DevDisconnected(&bsp::UsbCdcSerialPort::UsbdHandle());
							   });

	pcd->SetDataOutStageCallback([](base::usb::fs_pcd::DataOutStageCallbackArgs const &args)
								 {
									 USBD_LL_DataOutStage(&bsp::UsbCdcSerialPort::UsbdHandle(),
														  args.EndpointNumber(),
														  const_cast<uint8_t *>(args.Span().Buffer()));
								 });

	HAL_PCD_RegisterDataInStageCallback(&bsp::UsbFsPcd::HalPcdHandle(),
										PCD_DataInStageCallback);

	HAL_PCD_RegisterIsoOutIncpltCallback(&bsp::UsbFsPcd::HalPcdHandle(),
										 PCD_ISOOUTIncompleteCallback);

	HAL_PCD_RegisterIsoInIncpltCallback(&bsp::UsbFsPcd::HalPcdHandle(),
										PCD_ISOINIncompleteCallback);

	return USBD_OK;
}

/**
 * @brief  Starts the low level portion of the device driver.
 * @param  pdev: Device handle
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
	std::shared_ptr<base::usb::fs_pcd::UsbFsPcd> pcd = base::usb::fs_pcd::usb_fs_pcd_slot()[0];
	pcd->Start();

	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;
	usb_status = USBD_Get_USB_Status(hal_status);
	return usb_status;
}

/**
 * @brief  Stops the low level portion of the device driver.
 * @param  pdev: Device handle
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_Stop(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData));

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Opens an endpoint of the low level driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @param  ep_type: Endpoint type
 * @param  ep_mps: Endpoint max packet size
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t ep_type, uint16_t ep_mps)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Open(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, ep_mps, ep_type);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Closes an endpoint of the low level driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Close(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Flushes an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Flush(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_SetStall(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_ClrStall(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Returns Stall condition.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @retval Stall (1: Yes, 0: No)
 */
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef *)pdev->pData;

	if ((ep_addr & 0x80) == 0x80)
	{
		return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
	}
	else
	{
		return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
	}
}

/**
 * @brief  Assigns a USB address to the device.
 * @param  pdev: Device handle
 * @param  dev_addr: Device address
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_SetAddress(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), dev_addr);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Transmits data over an endpoint.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @param  pbuf: Pointer to data to be sent
 * @param  size: Data size
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t size)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Transmit(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Prepares an endpoint for reception.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @param  pbuf: Pointer to data to be received
 * @param  size: Data size
 * @retval USBD status
 */
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t size)
{
	HAL_StatusTypeDef hal_status = HAL_OK;
	USBD_StatusTypeDef usb_status = USBD_OK;

	hal_status = HAL_PCD_EP_Receive(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size);

	usb_status = USBD_Get_USB_Status(hal_status);

	return usb_status;
}

/**
 * @brief  Returns the last transferred packet size.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint number
 * @retval Received Data Size
 */
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t ep_addr)
{
	return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef *)pdev->pData, ep_addr);
}

#ifdef USBD_HS_TESTMODE_ENABLE
/**
 * @brief  Set High speed Test mode.
 * @param  pdev: Device handle
 * @param  testmode: test mode
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_SetTestMode(USBD_HandleTypeDef *pdev, uint8_t testmode)
{
	UNUSED(pdev);
	UNUSED(testmode);

	return USBD_OK;
}
#endif /* USBD_HS_TESTMODE_ENABLE */

/**
 * @brief  Delays routine for the USB device library.
 * @param  Delay: Delay in ms
 * @retval None
 */
void USBD_LL_Delay(uint32_t Delay)
{
	HAL_Delay(Delay);
}

/**
 * @brief  Returns the USB status depending on the HAL status:
 * @param  hal_status: HAL status
 * @retval USB status
 */
USBD_StatusTypeDef USBD_Get_USB_Status(HAL_StatusTypeDef hal_status)
{
	USBD_StatusTypeDef usb_status = USBD_OK;

	switch (hal_status)
	{
	case HAL_OK:
		usb_status = USBD_OK;
		break;
	case HAL_ERROR:
		usb_status = USBD_FAIL;
		break;
	case HAL_BUSY:
		usb_status = USBD_BUSY;
		break;
	case HAL_TIMEOUT:
		usb_status = USBD_FAIL;
		break;
	default:
		usb_status = USBD_FAIL;
		break;
	}
	return usb_status;
}

/**
 * @brief  Static single allocation.
 * @param  size: Size of allocated memory
 * @retval None
 */
void *USBD_static_malloc(uint32_t size)
{
	UNUSED(size);
	static uint32_t mem[(sizeof(USBD_CDC_HandleTypeDef) / 4) + 1]; /* On 32-bit boundary */
	return mem;
}

/**
 * @brief  Dummy memory free
 * @param  p: Pointer to allocated  memory address
 * @retval None
 */
void USBD_static_free(void *p)
{
	UNUSED(p);
}

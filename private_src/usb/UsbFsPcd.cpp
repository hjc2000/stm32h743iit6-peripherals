#include "UsbFsPcd.h"

void bsp::UsbFsPcd::InitializeCallback()
{
	_handle_context._handle.SOFCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnSofCallback();
	};

	_handle_context._handle.SetupStageCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnSetupStageCallback();
	};

	_handle_context._handle.ResetCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnResetCallback();
	};

	_handle_context._handle.SuspendCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnSuspendCallback();
	};

	_handle_context._handle.ResumeCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnResumeCallback();
	};

	_handle_context._handle.ConnectCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnConnectCallback();
	};

	_handle_context._handle.DisconnectCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnDisconnectCallback();
	};

	_handle_context._handle.DataOutStageCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnDataOutStageCallback(epnum);
	};

	_handle_context._handle.DataInStageCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnDataInStageCallback(epnum);
	};

	_handle_context._handle.ISOOUTIncompleteCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnISOOUTIncompleteCallback(epnum);
	};

	_handle_context._handle.ISOINIncompleteCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<handle_context *>(handle)->_self;
		self->OnISOINIncompleteCallback(epnum);
	};
}

void bsp::UsbFsPcd::InitializeAsDevice(std::string const &clock_source_name,
									   uint32_t divider,
									   base::usb_fs_pcd::PhyType phy_type)
{
	_handle_context._handle.Instance = USB_OTG_FS;
	_handle_context._handle.Init.dev_endpoints = 9;
	_handle_context._handle.Init.speed = PCD_SPEED_FULL;
	_handle_context._handle.Init.dma_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.phy_itface = PCD_PHY_EMBEDDED;
	_handle_context._handle.Init.Sof_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.low_power_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.lpm_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.battery_charging_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.vbus_sensing_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.use_dedicated_ep1 = FunctionalState::DISABLE;
	if (HAL_PCD_Init(&_handle_context._handle) != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	InitializeCallback();

	HAL_PCDEx_SetRxFiFo(&_handle_context._handle, 0x80);
	HAL_PCDEx_SetTxFiFo(&_handle_context._handle, 0, 0x40);
	HAL_PCDEx_SetTxFiFo(&_handle_context._handle, 1, 0x80);
}

#include "UsbFsPcd.h"
#include "base/embedded/interrupt/interrupt.h"
#include "stm32_hal_legacy.h"
#include <cstdint>
#include <functional>

/* #region 中断服务函数 */

std::function<void()> _usb_isr;

extern "C"
{
	void OTG_FS_IRQHandler(void)
	{
		if (_usb_isr)
		{
			_usb_isr();
		}
	}
}

/* #endregion */

void bsp::UsbFsPcd::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));

	_usb_isr = [this]()
	{
		HAL_PCD_IRQHandler(&_handle_context._handle);
	};
}

void bsp::UsbFsPcd::InitializeCallback()
{
	_handle_context._handle.SOFCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnSofCallback();
	};

	_handle_context._handle.SetupStageCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnSetupStageCallback();
	};

	_handle_context._handle.ResetCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnResetCallback();
	};

	_handle_context._handle.SuspendCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnSuspendCallback();
	};

	_handle_context._handle.ResumeCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnResumeCallback();
	};

	_handle_context._handle.ConnectCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnConnectCallback();
	};

	_handle_context._handle.DisconnectCallback = [](PCD_HandleTypeDef *handle)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnDisconnectCallback();
	};

	_handle_context._handle.DataOutStageCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnDataOutStageCallback(epnum);
	};

	_handle_context._handle.DataInStageCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnDataInStageCallback(epnum);
	};

	// 等时传输中，如果主机输出的数据没有及时被设备接收，就会触发此回调。
	//
	// 当主机试图发送数据给 stm32, 但是 stm32 的 USB 外设的端点接收 FIFO
	// 正在被占用，上一次的数据还没处理完，就会触发此回调。
	_handle_context._handle.ISOOUTIncompleteCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnIsoOutIncompleteCallback(epnum);
	};

	// 等时传输中，主机没有及时取走输入数据，就会触发此回调。
	//
	// 设备将准备给主机的数据放到 FIFO 中，等待主机取走，主机如果没有及时取走，
	// 就会触发此回调。但是设备不管，设备会放入新的数据，替换掉旧数据，旧数据直接丢包。
	_handle_context._handle.ISOINIncompleteCallback = [](PCD_HandleTypeDef *handle, uint8_t epnum)
	{
		UsbFsPcd *self = reinterpret_cast<hal_pcd_handle_context *>(handle)->_self;
		self->OnIsoInIncompleteCallback(epnum);
	};
}

void bsp::UsbFsPcd::Initialize(base::usb::PhyType phy_type)
{
	__HAL_RCC_USB_OTG_FS_CLK_ENABLE();
	HAL_PWREx_EnableUSBVoltageDetector();

	_handle_context._handle.Instance = USB_OTG_FS;
	_handle_context._handle.Init.dev_endpoints = 9;
	_handle_context._handle.Init.speed = PCD_SPEED_FULL;
	_handle_context._handle.Init.dma_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.phy_itface = PCD_PHY_EMBEDDED;
	_handle_context._handle.Init.Sof_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.low_power_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.lpm_enable = FunctionalState::DISABLE;

	/// @brief 在 cubemx 中使能 VBUS 后，battery_charging_enable 和 vbus_sensing_enable
	/// 都会变成 ENABLE.
	///
	/// 此时还会额外初始化 PA9 引脚。这个引脚配置为输入模式，用来检测 VBUS 当前是否有效。
	/// 如果没有配置 PA9 引脚的话，绝对不能将 battery_charging_enable 和 vbus_sensing_enable
	/// 设置为 ENABLE.
	///
	/// 此外，PA9 引脚不能直接连接到 USB 接口的 VBUS 引脚，因为 PA9 配置为输入模式只能承受 3.3V 输入，
	/// 这么做会烧毁引脚。要使用一个数字比较器，或者分压电阻，当 VBUS 有效时输出高电平，高电平进入 PA9
	/// 引脚，STM32 的 USB 外设检测到了就认为 VBUS 有效。
	///
	/// @warning PA9 作为 VBUS 功能配置的是普通的输入模式，无上下拉。USB 外设直接读取输入寄存器的值。
	/// PA9 不需要配置为 VBUS 复用功能，实际上也没有 VBUS 复用功能。VBUS 检测不是通过复用来实现的。
	///
	/// HAL_PWREx_EnableUSBVoltageDetector 随便调用，没事，即使没有使能 VBUS 检测功能。因为 cubemx
	/// 生成的代码也是不管有没有使能 VBUS 检测功能，始终调用 HAL_PWREx_EnableUSBVoltageDetector.
	///
	/// @note 资料
	/// 	@li stm32h743iit6 USB FS 启用 VBUS 或 BCD 前后的区别
	/// 		https://blog.csdn.net/qq_36148047/article/details/152078209?sharetype=blogdetail&sharerId=152078209&sharerefer=PC&sharesource=qq_36148047&spm=1011.2480.3001.8118
	///
	_handle_context._handle.Init.battery_charging_enable = FunctionalState::DISABLE;
	_handle_context._handle.Init.vbus_sensing_enable = FunctionalState::DISABLE;

	_handle_context._handle.Init.use_dedicated_ep1 = FunctionalState::DISABLE;

	HAL_StatusTypeDef result = HAL_PCD_Init(&_handle_context._handle);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	InitializeInterrupt();
	InitializeCallback();

	HAL_PCDEx_SetRxFiFo(&_handle_context._handle, 0x80);
	HAL_PCDEx_SetTxFiFo(&_handle_context._handle, 0, 0x40);
	HAL_PCDEx_SetTxFiFo(&_handle_context._handle, 1, 0x80);
}

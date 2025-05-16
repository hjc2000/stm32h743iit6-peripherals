#pragma once
#include "base/embedded/gpio/gpio_handle.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPinUsageStateManager.h"
#include "base/embedded/interrupt/exti.h"
#include "gpio.h"
#include "hal.h"
#include <cstdint>

namespace bsp
{
	///
	/// @brief 将端口枚举转为 HAL 库的端口实例指针。
	///
	/// @param value
	/// @return constexpr GPIO_TypeDef*
	///
	constexpr GPIO_TypeDef *ToPort(base::gpio::PortEnum value)
	{
		switch (value)
		{
		case base::gpio::PortEnum::PortA:
			{
				return GPIOA;
			}
		case base::gpio::PortEnum::PortB:
			{
				return GPIOB;
			}
		case base::gpio::PortEnum::PortC:
			{
				return GPIOC;
			}
		case base::gpio::PortEnum::PortD:
			{
				return GPIOD;
			}
		case base::gpio::PortEnum::PortE:
			{
				return GPIOE;
			}
		case base::gpio::PortEnum::PortF:
			{
				return GPIOF;
			}
		case base::gpio::PortEnum::PortG:
			{
				return GPIOG;
			}
		case base::gpio::PortEnum::PortH:
			{
				return GPIOH;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR};
			}
		}
	}

	///
	/// @brief 将引脚号转为 HAL 库的引脚宏定义。
	///
	/// @param pin
	/// @return constexpr uint32_t
	///
	constexpr uint32_t ToPinDefineValue(uint32_t pin)
	{
		switch (pin)
		{
		case 0:
			{
				return GPIO_PIN_0;
			}
		case 1:
			{
				return GPIO_PIN_1;
			}
		case 2:
			{
				return GPIO_PIN_2;
			}
		case 3:
			{
				return GPIO_PIN_3;
			}
		case 4:
			{
				return GPIO_PIN_4;
			}
		case 5:
			{
				return GPIO_PIN_5;
			}
		case 6:
			{
				return GPIO_PIN_6;
			}
		case 7:
			{
				return GPIO_PIN_7;
			}
		case 8:
			{
				return GPIO_PIN_8;
			}
		case 9:
			{
				return GPIO_PIN_9;
			}
		case 10:
			{
				return GPIO_PIN_10;
			}
		case 11:
			{
				return GPIO_PIN_11;
			}
		case 12:
			{
				return GPIO_PIN_12;
			}
		case 13:
			{
				return GPIO_PIN_13;
			}
		case 14:
			{
				return GPIO_PIN_14;
			}
		case 15:
			{
				return GPIO_PIN_15;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法引脚号。"};
			}
		}
	}

} // namespace bsp

class base::gpio::gpio_pin_handle
{
private:
	base::gpio::GpioPinUsageStateManager<static_cast<uint32_t>(base::gpio::PortEnum::PortH) + 1, 16> _usage_state_manager;
	base::gpio::PortEnum _port_enum{};
	GPIO_TypeDef *_port = nullptr;
	uint32_t _pin = 0;
	uint32_t _pin_define = 0;

	uint32_t GetAlternateFunctionDefineValue(base::gpio::AlternateFunction af)
	{
		switch (af)
		{
		case base::gpio::AlternateFunction::UART1:
			{
				return GPIO_AF7_USART1;
			}
		case base::gpio::AlternateFunction::SDRAM1:
			{
				return GPIO_AF12_FMC;
			}
		case base::gpio::AlternateFunction::TIMER3:
			{
				return GPIO_AF2_TIM3;
			}
		default:
			{
				return 0;
			}
		}
	}

public:
	gpio_pin_handle(base::gpio::PortEnum port, uint32_t pin)
		: _usage_state_manager(port, pin)
	{
		_port_enum = port;
		_port = bsp::ToPort(port);
		_pin = pin;
		_pin_define = bsp::ToPinDefineValue(pin);
	}

	/* #region 初始化方法 */

	void InitializeAsInputMode(base::gpio::PullMode pull_mode,
							   base::gpio::TriggerEdge trigger_edge)
	{
		bsp::gpio::enable_clock(_port);
		GPIO_InitTypeDef def{};
		switch (pull_mode)
		{
		default:
		case base::gpio::PullMode::NoPull:
			{
				def.Pull = GPIO_NOPULL;
				break;
			}
		case base::gpio::PullMode::PullUp:
			{
				def.Pull = GPIO_PULLUP;
				break;
			}
		case base::gpio::PullMode::PullDown:
			{
				def.Pull = GPIO_PULLDOWN;
				break;
			}
		}

		switch (trigger_edge)
		{
		default:
		case base::gpio::TriggerEdge::Disable:
			{
				def.Mode = GPIO_MODE_INPUT;
				break;
			}
		case base::gpio::TriggerEdge::RisingEdge:
			{
				def.Mode = GPIO_MODE_IT_RISING;
				break;
			}
		case base::gpio::TriggerEdge::FallingEdge:
			{
				def.Mode = GPIO_MODE_IT_FALLING;
				break;
			}
		case base::gpio::TriggerEdge::BothEdge:
			{
				def.Mode = GPIO_MODE_IT_RISING_FALLING;
				break;
			}
		}

		def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		def.Pin = _pin_define;
		HAL_GPIO_Init(_port, &def);
	}

	void InitializeAsInputMode()
	{
		InitializeAsInputMode(base::gpio::PullMode::PullUp, base::gpio::TriggerEdge::Disable);
	}

	void InitializeAsOutputMode(base::gpio::PullMode pull_mode,
								base::gpio::DriveMode drive_mode)
	{
		bsp::gpio::enable_clock(_port);
		GPIO_InitTypeDef def{};
		switch (pull_mode)
		{
		default:
		case base::gpio::PullMode::NoPull:
			{
				def.Pull = GPIO_NOPULL;
				break;
			}
		case base::gpio::PullMode::PullUp:
			{
				def.Pull = GPIO_PULLUP;
				break;
			}
		case base::gpio::PullMode::PullDown:
			{
				def.Pull = GPIO_PULLDOWN;
				break;
			}
		}

		switch (drive_mode)
		{
		case base::gpio::DriveMode::PushPull:
			{
				def.Mode = GPIO_MODE_OUTPUT_PP;
				break;
			}
		case base::gpio::DriveMode::OpenDrain:
			{
				def.Mode = GPIO_MODE_OUTPUT_OD;
				break;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "不支持的驱动模式。"};
			}
		}

		def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		def.Pin = _pin_define;
		HAL_GPIO_Init(_port, &def);
	}

	void InitializeAsOutputMode()
	{
		InitializeAsOutputMode(base::gpio::PullMode::PullUp, base::gpio::DriveMode::PushPull);
	}

	void InitializeAsAlternateFunctionMode(base::gpio::AlternateFunction af,
										   base::gpio::PullMode pull_mode,
										   base::gpio::DriveMode drive_mode)
	{
		bsp::gpio::enable_clock(_port);
		GPIO_InitTypeDef def{};
		def.Alternate = GetAlternateFunctionDefineValue(af);

		switch (pull_mode)
		{
		default:
		case base::gpio::PullMode::NoPull:
			{
				def.Pull = GPIO_NOPULL;
				break;
			}
		case base::gpio::PullMode::PullUp:
			{
				def.Pull = GPIO_PULLUP;
				break;
			}
		case base::gpio::PullMode::PullDown:
			{
				def.Pull = GPIO_PULLDOWN;
				break;
			}
		}

		switch (drive_mode)
		{
		case base::gpio::DriveMode::PushPull:
			{
				def.Mode = GPIO_MODE_AF_PP;
				break;
			}
		case base::gpio::DriveMode::OpenDrain:
			{
				def.Mode = GPIO_MODE_AF_OD;
				break;
			}
		default:
			{
				throw std::invalid_argument{"不支持的 Driver"};
			}
		}

		def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		def.Pin = _pin_define;
		HAL_GPIO_Init(_port, &def);
	}

	/* #endregion */

	/* #region 引脚信息 */

	base::gpio::PortEnum Port() const
	{
		return _port_enum;
	}

	uint32_t Pin() const
	{
		return _pin;
	}

	/* #endregion */

	/* #region 读写引脚方法 */

	bool ReadPin()
	{
		GPIO_PinState pin_state = HAL_GPIO_ReadPin(_port, _pin_define);
		if (pin_state == GPIO_PinState::GPIO_PIN_SET)
		{
			return true;
		}

		return false;
	}

	void WritePin(bool value)
	{
		GPIO_PinState state = value ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET;
		HAL_GPIO_WritePin(_port, _pin_define, state);
	}

	void TogglePin()
	{
		HAL_GPIO_TogglePin(_port, _pin_define);
	}

	/* #endregion */

	/* #region 中断回调 */

	void RegisterInterruptCallback(int32_t priority, std::function<void()> const &callback_func)
	{
		base::exti::register_callback(_pin, priority, callback_func);
	}

	void RegisterInterruptCallback(std::function<void()> const &callback_func)
	{
		base::exti::register_callback(_pin, callback_func);
	}

	void UnregisterInterruptCallback()
	{
		base::exti::unregister_callback(_pin);
	}

	/* #endregion */
};

#pragma once
#include "base/embedded/gpio/gpio_handle.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPinUsageStateManager.h"
#include "base/embedded/interrupt/exti.h"
#include "gpio.h"
#include "hal.h"
#include <cstdint>

class base::gpio::gpio_pin_handle
{
private:
	base::gpio::GpioPinUsageStateManager<static_cast<uint32_t>(base::gpio::PortEnum::PortH) + 1, 16> _usage_state_manager;
	base::gpio::PortEnum _port_enum{};
	GPIO_TypeDef *_port = nullptr;
	uint32_t _pin = 0;
	uint32_t _pin_define = 0;

public:
	gpio_pin_handle(base::gpio::PortEnum port, uint32_t pin)
		: _usage_state_manager(port, pin)
	{
		_port_enum = port;
		_port = bsp::gpio::to_defined_value(port);
		_pin = pin;
		_pin_define = bsp::gpio::to_pin_define_value(pin);
	}

	/* #region 初始化方法 */

	void InitializeAsInputMode(base::gpio::PullMode pull_mode,
							   base::gpio::TriggerEdge trigger_edge)
	{
		bsp::gpio::enable_clock(_port);
		GPIO_InitTypeDef def{};
		def.Pull = bsp::gpio::to_defined_value(pull_mode);

		switch (trigger_edge)
		{
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
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法 TriggerEdge."};
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
		def.Pull = bsp::gpio::to_defined_value(pull_mode);

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
		def.Alternate = bsp::gpio::to_defined_value(af);
		def.Pull = bsp::gpio::to_defined_value(pull_mode);

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

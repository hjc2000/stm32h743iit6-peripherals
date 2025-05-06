#include "gpio_pin_handle.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/interrupt/exti.h"
#include "base/string/define.h"
#include "stm32h7xx_hal_gpio.h"
#include <cstdint>
#include <stdexcept>

/* #region 初始化帮助方法 */

void base::gpio::gpio_pin_handle::EnableClock()
{
	if (_port == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (_port == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (_port == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (_port == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if (_port == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if (_port == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if (_port == GPIOG)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	else if (_port == GPIOH)
	{
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
}

uint32_t base::gpio::gpio_pin_handle::GetAlternateFunctionDefineValue(base::gpio::AlternateFunction af)
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

/* #endregion */

base::gpio::gpio_pin_handle::gpio_pin_handle(base::gpio::PortEnum port, uint32_t pin)
	: _usage_state_manager(port, pin)
{
	_port_enum = port;
	_port = bsp::ToPort(port);
	_pin = pin;
	_pin_define = bsp::ToPinDefineValue(pin);
}

/* #region 初始化方法 */

void base::gpio::gpio_pin_handle::InitializeAsInputMode(base::gpio::PullMode pull_mode,
														base::gpio::TriggerEdge trigger_edge)
{
	EnableClock();
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

void base::gpio::gpio_pin_handle::InitializeAsOutputMode(base::gpio::PullMode pull_mode,
														 base::gpio::DriveMode drive_mode)
{
	EnableClock();
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

void base::gpio::gpio_pin_handle::InitializeAsAlternateFunctionMode(base::gpio::AlternateFunction af,
																	base::gpio::PullMode pull_mode,
																	base::gpio::DriveMode drive_mode)
{
	EnableClock();
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

/* #region 读写引脚方法 */

bool base::gpio::gpio_pin_handle::ReadPin()
{
	GPIO_PinState pin_state = HAL_GPIO_ReadPin(_port, _pin_define);
	if (pin_state == GPIO_PinState::GPIO_PIN_SET)
	{
		return true;
	}

	return false;
}

void base::gpio::gpio_pin_handle::WritePin(bool value)
{
	GPIO_PinState state = value ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET;
	HAL_GPIO_WritePin(_port, _pin_define, state);
}

void base::gpio::gpio_pin_handle::TogglePin()
{
	HAL_GPIO_TogglePin(_port, _pin_define);
}

/* #endregion */

/* #region 中断回调 */

void base::gpio::gpio_pin_handle::RegisterInterruptCallback(std::function<void()> const &callback_func)
{
	base::exti::register_callback(_pin, callback_func);
}

void base::gpio::gpio_pin_handle::UnregisterInterruptCallback()
{
	base::exti::unregister_callback(_pin);
}

/* #endregion */

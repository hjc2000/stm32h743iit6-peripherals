#pragma once
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/string/define.h"
#include "hal.h"
#include <stdexcept>

namespace bsp
{
	namespace gpio
	{
		void enable_clock(GPIO_TypeDef *port);

		constexpr uint32_t to_defined_value(base::gpio::AlternateFunction af)
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
					throw std::invalid_argument{CODE_POS_STR + "非法复用功能。"};
				}
			}
		}

	} // namespace gpio
} // namespace bsp

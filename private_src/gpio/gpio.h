#pragma once
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/string/define.h"
#include "hal.h"
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	namespace gpio
	{
		inline void enable_clock(GPIO_TypeDef *port)
		{
			if (port == GPIOA)
			{
				__HAL_RCC_GPIOA_CLK_ENABLE();
			}
			else if (port == GPIOB)
			{
				__HAL_RCC_GPIOB_CLK_ENABLE();
			}
			else if (port == GPIOC)
			{
				__HAL_RCC_GPIOC_CLK_ENABLE();
			}
			else if (port == GPIOD)
			{
				__HAL_RCC_GPIOD_CLK_ENABLE();
			}
			else if (port == GPIOE)
			{
				__HAL_RCC_GPIOE_CLK_ENABLE();
			}
			else if (port == GPIOF)
			{
				__HAL_RCC_GPIOF_CLK_ENABLE();
			}
			else if (port == GPIOG)
			{
				__HAL_RCC_GPIOG_CLK_ENABLE();
			}
			else if (port == GPIOH)
			{
				__HAL_RCC_GPIOH_CLK_ENABLE();
			}
		}

		///
		/// @brief 将端口枚举转为 HAL 库的端口实例指针。
		///
		/// @param value
		/// @return
		///
		constexpr GPIO_TypeDef *to_defined_value(base::gpio::PortEnum value)
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
					throw std::invalid_argument{CODE_POS_STR + "非法端口。"};
				}
			}
		}

		///
		/// @brief 将引脚号转为 HAL 库的引脚宏定义。
		///
		/// @param pin
		/// @return
		///
		constexpr uint32_t to_pin_define_value(uint32_t pin)
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

		constexpr uint32_t to_defined_value(base::gpio::PullMode pull_mode)
		{
			switch (pull_mode)
			{
			case base::gpio::PullMode::NoPull:
				{
					return GPIO_NOPULL;
				}
			case base::gpio::PullMode::PullUp:
				{
					return GPIO_PULLUP;
				}
			case base::gpio::PullMode::PullDown:
				{
					return GPIO_PULLDOWN;
				}
			default:
				{
					throw std::invalid_argument{CODE_POS_STR + "非法 PullMode"};
				}
			}
		}

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

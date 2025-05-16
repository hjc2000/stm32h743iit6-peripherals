#include "gpio_pin_handle.h"
#include "base/embedded/gpio/gpio_parameter.h"
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

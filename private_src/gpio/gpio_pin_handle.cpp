#include "gpio_pin_handle.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include <cstdint>

/* #region 初始化帮助方法 */

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

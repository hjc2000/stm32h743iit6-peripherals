#include "hal.h"

extern "C"
{
	uint32_t freertos_get_systic_clock_freq(uint8_t sync_to_cpu)
	{
		uint32_t freq = HAL_RCC_GetSysClockFreq();
		if (!sync_to_cpu)
		{
			// 这里不能检查 SysTick->CTRL 的 bit2 来决定返回 HCLK 的频率还是返回 HCLK / 8,
			// 因为 freertos 调用本函数的时候还没设置 SysTick->CTRL 的 bit2, 调用完后会
			// 设置 SysTick->CTRL 的 bit2.
			freq /= 8;
		}

		return freq;
	}
}

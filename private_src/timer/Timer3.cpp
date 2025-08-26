#include "Timer3.h" // IWYU pragma: keep
#include "Pclk1ClockSignal.h"

base::unit::MHz bsp::Timer3::ClockSourceFrequency()
{
	// 定时器 3 的时钟源是 PCLK1 再经过一个倍频器。当 PCLK1 的输入分频系数大于 1
	// 时，这个倍频器会把 PCLK1 的频率乘 2, 如果 PCLK1 的输入分频系数为 1, 则不
	// 倍频。
	bsp::Pclk1ClockSignal pclk1{};
	base::unit::MHz freq = pclk1.Frequency();
	if (pclk1.InputDivider() > 1)
	{
		freq *= 2;
	}

	return freq;
}

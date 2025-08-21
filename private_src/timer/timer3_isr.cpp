#include "timer3_isr.h" // IWYU pragma: keep
#include <functional>

namespace
{
	std::function<void()> _timer3_isr;

}

void bsp::set_timer3_isr(std::function<void()> func)
{
}

extern "C"
{
	void TIM3_IRQHandler()
	{
		if (_timer3_isr == nullptr)
		{
			return;
		}

		_timer3_isr();
	}
}

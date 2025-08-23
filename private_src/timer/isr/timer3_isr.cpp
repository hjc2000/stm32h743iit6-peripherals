#include "timer3_isr.h" // IWYU pragma: keep
#include "base/embedded/interrupt/interrupt.h"
#include "hal.h" // IWYU pragma: keep
#include <functional>

namespace
{
	std::function<void()> _timer3_isr;

}

void bsp::set_timer3_isr(std::function<void()> func)
{
	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM3_IRQn));
	_timer3_isr = func;
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

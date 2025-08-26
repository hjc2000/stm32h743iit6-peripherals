#include "timer5_isr.h" // IWYU pragma: keep
#include "base/embedded/interrupt/interrupt.h"
#include "hal.h" // IWYU pragma: keep
#include <functional>

namespace
{
	std::function<void()> _timer5_isr;

}

void bsp::set_timer5_isr(std::function<void()> func)
{
	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM5_IRQn));
	_timer5_isr = func;
}

extern "C"
{
	void TIM5_IRQHandler()
	{
		if (_timer5_isr == nullptr)
		{
			return;
		}

		_timer5_isr();
	}
}

#include "base/embedded/systick/systick.h"
#include "base/bit/bit.h"
#include "base/embedded/interrupt/interrupt.h"
#include "base/task/delay.h"
#include "base/unit/Hz.h"
#include "base/unit/MHz.h"
#include "base/unit/Nanoseconds.h"
#include "base/unit/Seconds.h"
#include "hal.h"
#include <chrono>
#include <cstdint>

namespace
{
	std::function<void()> _elapsed_handler{};
	uint64_t _total_tick{};

} // namespace

base::MHz base::systick::frequency()
{
	base::MHz ret{base::Hz{HAL_RCC_GetSysClockFreq()}};
	if (base::bit::ReadBit(SysTick->CTRL, 2) == 0)
	{
		ret /= 8;
	}

	return ret;
}

uint64_t base::systick::current_value()
{
	uint32_t masked = SysTick->VAL & SysTick_VAL_CURRENT_Msk;
	return masked >> SysTick_VAL_CURRENT_Pos;
}

uint64_t base::systick::reload_value()
{
	uint32_t masked = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;
	return masked >> SysTick_LOAD_RELOAD_Pos;
}

void base::systick::set_elapsed_handler(std::function<void()> func)
{
	base::interrupt::GlobalInterruptionGuard g;
	_elapsed_handler = func;
}

///
/// @brief 系统时间戳。
///
/// @return
///
base::Nanoseconds base::systick::system_time_stamp()
{
	base::Nanoseconds clock_period{base::systick::frequency()};
	base::Nanoseconds elapsed_period = clock_period * base::systick::reload_value();
	return _total_tick * elapsed_period;
}

extern "C"
{
	/// @brief 重写 __weak 的 HAL_Delay 函数
	/// @param ms 要延时的毫秒数。
	void HAL_Delay(uint32_t ms)
	{
		base::task::Delay(std::chrono::milliseconds{ms});
	}

	void SysTick_Handler()
	{
		// 读取一下 CTRL，SysTick 的溢出标志位会自动清除。
		SysTick->CTRL;
		HAL_IncTick();
		_total_tick++;

		if (_elapsed_handler != nullptr)
		{
			_elapsed_handler();
		}
	}
}

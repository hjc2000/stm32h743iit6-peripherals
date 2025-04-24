#include "base/peripheral/systick/systick.h"
#include "base/task/delay.h"
#include "base/unit/Hz.h"
#include "base/unit/MHz.h"
#include "base/unit/Nanoseconds.h"
#include "base/unit/Seconds.h"
#include "bsp-interface/di/interrupt.h"
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
	// stm32h743 不支持 8 分频。
	return base::MHz{base::Hz{HAL_RCC_GetSysClockFreq()}};
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
	bsp::di::interrupt::GlobalInterruptGuard g;
	_elapsed_handler = func;
}

///
/// @brief 系统时间戳。
///
/// @return
///
base::Nanoseconds base::systick::system_time_stamp()
{
	base::Nanoseconds tick_period{base::systick::frequency()};
	uint64_t period_count = _total_tick / base::systick::reload_value();
	return period_count * tick_period;
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
		_total_tick += base::systick::reload_value();

		if (_elapsed_handler != nullptr)
		{
			_elapsed_handler();
		}
	}
}

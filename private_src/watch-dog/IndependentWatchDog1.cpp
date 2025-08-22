#include "IndependentWatchDog1.h" // IWYU pragma: keep
#include "base/string/define.h"
#include <stdexcept>

void bsp::IndependentWatchDog1::Initialize(std::chrono::milliseconds const &timeout)
{
	_timeout = timeout;
	_handle.Instance = IWDG1;

	// 将窗口设置为计数器最大值，这样只要不超时，何时喂狗都可以，没有窗口的限制。
	_handle.Init.Window = 0x0fff;

	base::unit::Second inner_clock_source_interval{_inner_clock_frequency};

	// 所需的 (分频器计数值 + 计数器计数值)
	int64_t total_count = static_cast<int64_t>(base::unit::Second{timeout} / inner_clock_source_interval);
	if (total_count <= 0)
	{
		// 超时时间短得都小于看门狗时钟源周期了。
		throw std::invalid_argument{CODE_POS_STR + "超时时间过短，无法满足。"};
	}

	for (size_t i = 0; i < _prescaler_defines_count; i++)
	{
		// 不断尝试将剩余计数交给计数器，计数器承受不了就继续下一轮循环，
		// 下一轮循环会增大分频系数。
		int64_t remain_count = total_count / GetPrescalerFactor(i);
		_handle.Init.Prescaler = _prescaler_defines[i];

		if (remain_count <= 0x0fff)
		{
			_handle.Init.Reload = remain_count;
			break;
		}

		// remain_count > 0x0fff
		// 分频器分频后，剩余的计数交给计数器，计数器承受不了。
		if (_prescaler_defines[i] == IWDG_PRESCALER_256)
		{
			// 已经达到最大分频了
			// 让计数器满足这么大的超时时间的要求是绝不可能的。
			throw std::invalid_argument{CODE_POS_STR + "超时时间过长，无法满足。"};
		}
	}

	HAL_IWDG_Init(&_handle);
}

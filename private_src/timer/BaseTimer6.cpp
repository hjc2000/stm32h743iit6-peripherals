#include "BaseTimer6.h" // IWYU pragma: keep
#include "base/bit/bit.h"
#include "base/embedded/interrupt/interrupt.h"
#include "base/math/pow.h"
#include "base/string/define.h"
#include "base/unit/Nanosecond.h"
#include "Pclk1ClockSignal.h"
#include <cstdint>
#include <functional>
#include <stdexcept>

namespace
{
	std::function<void()> _timer6_isr;

}

extern "C"
{
	void TIM6_DAC_IRQHandler()
	{
		if (_timer6_isr == nullptr)
		{
			return;
		}

		_timer6_isr();
	}
}

base::unit::MHz bsp::BaseTimer6::ClockSourceFrequency() const
{
	// 定时器 6 的时钟源是 PCLK1 再经过一个倍频器。当 PCLK1 的输入分频系数大于 1
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

void bsp::BaseTimer6::InitializePeriod(std::chrono::nanoseconds const &period)
{
	if (period.count() <= 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "定时周期不能 <= 0."};
	}

	base::unit::Nanosecond clock_source_cycle{ClockSourceFrequency()};
	uint64_t const total_cycle_count = static_cast<uint64_t>(base::unit::Nanosecond{period} / clock_source_cycle);
	if (total_cycle_count <= 0)
	{
		throw std::invalid_argument{CODE_POS_STR + "定时周期过小，小于分辨率。"};
	}

	if (total_cycle_count > base::pow<uint64_t>(2, 32))
	{
		// 超出范围了，这么大的计数，分频器和计数器都计数到最大值也还是小于。
		throw std::invalid_argument{CODE_POS_STR + "时间间隔过大，定时器无法满足。"};
	}

	// 计数器的进制数。
	// 即计数器能从 0 计数到 counter_base -1.
	uint64_t counter_base = total_cycle_count;

	// 分频系数，或者说是 counter_base 的倍数。即要计数多少个 counter_base 的值
	// 才能达到 total_cycle_count.
	//
	// 即
	// 		multiple * counter_base = total_cycle_count
	// 这个表达式需要始终成立。
	uint64_t multiple = 1;

	uint32_t lowest_one_bit_index = base::bit::LowestOneBitIndex(counter_base);
	if (lowest_one_bit_index > 0)
	{
		// 比如说 lowest_one_bit_index = 1, 即最低位的 1 的索引是 1, 则至少需要 2 分频，
		multiple <<= lowest_one_bit_index;
		counter_base >>= lowest_one_bit_index;
	}

	// 处理 counter_base 是 5 的整数倍的情况。
	uint64_t factor = 5;
	while (counter_base > factor && counter_base % factor == 0)
	{
		if (multiple * factor >= base::pow<uint64_t>(2, 16))
		{
			break;
		}

		counter_base /= factor;
		multiple *= factor;
	}

	// 处理 counter_base 是 3 的整数倍的情况。
	factor = 3;
	while (counter_base > factor && counter_base % factor == 0)
	{
		if (multiple * factor >= base::pow<uint64_t>(2, 16))
		{
			break;
		}

		counter_base /= factor;
		multiple *= factor;
	}

	// 剩下的 counter_base 如果还大于计数器的承受范围，就很有可能无法在不损失精度的情况下通过
	// 分频来达到定时周期了。
	if (counter_base > base::pow<uint64_t>(2, 16))
	{
		throw std::out_of_range{CODE_POS_STR + "无法在不损失定时精度的情况下定时。"};
	}

	_handle_context._handle.Init.Prescaler = multiple - 1;
	_handle_context._handle.Init.Period = counter_base - 1;
}

void bsp::BaseTimer6::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM6_DAC_IRQn));

	_timer6_isr = [this]()
	{
		HAL_TIM_IRQHandler(&_handle_context._handle);
	};

	// 先不启用中断，等到 SetPeriodElapsedCallback 函数被调用时再启用。
}

void bsp::BaseTimer6::Initialize(std::chrono::nanoseconds const &period)
{
	_period = period;

	__HAL_RCC_TIM6_CLK_ENABLE();
	_handle_context._handle.Instance = TIM6;
	_handle_context._handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	_handle_context._handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	InitializePeriod(period);

	if (HAL_TIM_Base_Init(&_handle_context._handle) != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	_handle_context._handle.PeriodElapsedCallback = [](TIM_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->OnPeriodElapsedCallback();
	};

	TIM_MasterConfigTypeDef sMasterConfig{};
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&_handle_context._handle, &sMasterConfig) != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	InitializeInterrupt();
}

void bsp::BaseTimer6::SetPeriod(std::chrono::nanoseconds const &period)
{
	InitializePeriod(period);
	TIM_Base_SetConfig(_handle_context._handle.Instance, &_handle_context._handle.Init);
}

#include "InputCaptureTimer3.h" // IWYU pragma: keep
#include "base/embedded/interrupt/interrupt.h"
#include "base/math/FactorExtractor.h"
#include "base/math/pow.h"
#include "base/unit/Nanosecond.h"
#include "timer3_isr.h"

void bsp::InputCaptureTimer3::InitializePeriod(std::chrono::nanoseconds const &period)
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

	base::FactorExtractor<uint64_t> factor_extractor{total_cycle_count};
	factor_extractor.ExtractConservatively(2, base::pow<uint64_t>(2, 16));
	factor_extractor.ExtractConservatively(3, base::pow<uint64_t>(2, 16));
	factor_extractor.ExtractConservatively(5, base::pow<uint64_t>(2, 16));

	if (factor_extractor.Factor() > base::pow<uint64_t>(2, 16))
	{
		throw std::out_of_range{CODE_POS_STR + "无法在不损失定时精度的情况下定时。"};
	}

	if (factor_extractor.Base() > base::pow<uint64_t>(2, 16))
	{
		throw std::out_of_range{CODE_POS_STR + "无法在不损失定时精度的情况下定时。"};
	}

	_handle_context._handle.Init.Prescaler = factor_extractor.Factor() - 1;
	_handle_context._handle.Init.Period = factor_extractor.Base() - 1;
	_period = period;
}

void bsp::InputCaptureTimer3::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM3_IRQn));

	set_timer3_isr(
		[this]()
		{
			HAL_TIM_IRQHandler(&_handle_context._handle);
		});

	// 先不启用中断，等到 SetPeriodElapsedCallback 函数被调用时再启用。
}

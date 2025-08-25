#include "PwmTimer3.h" // IWYU pragma: keep
#include "base/math/FactorExtractor.h"
#include "base/math/pow.h"
#include "base/string/define.h"
#include "base/unit/Nanosecond.h"
#include "Pclk1ClockSignal.h"
#include <cstdint>
#include <stdexcept>

base::unit::MHz bsp::PwmTimer3::ClockSourceFrequency() const
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

void bsp::PwmTimer3::InitializePeriod(std::chrono::nanoseconds const &period)
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
	factor_extractor.Extract(2, base::pow<uint64_t>(2, 16));
	factor_extractor.Extract(3, base::pow<uint64_t>(2, 16));
	factor_extractor.Extract(5, base::pow<uint64_t>(2, 16));

	if (factor_extractor.Base() > base::pow<uint64_t>(2, 16))
	{
		throw std::out_of_range{CODE_POS_STR + "无法在不损失定时精度的情况下定时。"};
	}

	_handle_context._handle.Init.Prescaler = factor_extractor.Factor() - 1;
	_handle_context._handle.Init.Period = factor_extractor.Base() - 1;
}

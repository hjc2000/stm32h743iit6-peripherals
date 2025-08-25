#include "PwmTimer3.h" // IWYU pragma: keep
#include "base/embedded/timer/pwm_timer_handle.h"
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

void bsp::PwmTimer3::InitializeAsUpMode(base::unit::Hz const &frequency)
{
	__HAL_RCC_TIM3_CLK_ENABLE();
	_handle_context._handle.Instance = TIM3;
	_handle_context._handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	_handle_context._handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	base::unit::Nanosecond period{frequency};
	if (period < base::unit::Nanosecond{1})
	{
		throw std::invalid_argument{CODE_POS_STR + "频率过高。"};
	}

	InitializePeriod(static_cast<std::chrono::nanoseconds>(period));

	if (HAL_TIM_PWM_Init(&_handle_context._handle) != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	TIM_MasterConfigTypeDef sMasterConfig{};
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&_handle_context._handle, &sMasterConfig) != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	// 配置 PWM 输出
	{
		// _output_configuration.OCMode = TIM_OCMODE_PWM1;
		// _output_configuration.Pulse = 0;

		// if (effective_polarity == base::pwm_timer::Polarity::Negative)
		// {
		// 	// 有效状态是低电平
		// 	_output_configuration.OCPolarity = TIM_OCPOLARITY_LOW;

		// 	// 空闲时是无效状态，输出高电平
		// 	_output_configuration.OCIdleState = TIM_OCIDLESTATE_SET;
		// }
		// else
		// {
		// 	// 有效状态是高电平
		// 	_output_configuration.OCPolarity = TIM_OCPOLARITY_HIGH;

		// 	// 空闲时是无效状态，输出低电平
		// 	_output_configuration.OCIdleState = TIM_OCIDLESTATE_RESET;
		// }
	}
}

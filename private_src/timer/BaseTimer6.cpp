#include "BaseTimer6.h" // IWYU pragma: keep
#include "base/embedded/interrupt/interrupt.h"
#include "base/math/FactorExtractor.h"
#include "base/math/math.h"
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

	HAL_StatusTypeDef result = HAL_TIM_Base_Init(&_handle_context._handle);
	if (result != HAL_OK)
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

	result = HAL_TIMEx_MasterConfigSynchronization(&_handle_context._handle, &sMasterConfig);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	InitializeInterrupt();
}

void bsp::BaseTimer6::SetPeriod(std::chrono::nanoseconds const &value)
{
	InitializePeriod(value);
	TIM_Base_SetConfig(_handle_context._handle.Instance, &_handle_context._handle.Init);
}

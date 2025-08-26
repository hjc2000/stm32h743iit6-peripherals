#include "PwmTimer3.h" // IWYU pragma: keep
#include "base/embedded/timer/pwm_timer_handle.h"
#include "base/math/FactorExtractor.h"
#include "base/math/pow.h"
#include "base/string/define.h"
#include "base/unit/Nanosecond.h"
#include "define.h"
#include <cstdint>
#include <stdexcept>

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

void bsp::PwmTimer3::InitializeAsUpMode(base::unit::Hz const &frequency)
{
	_handle_context._handle.Instance = TIM3;
	_handle_context._handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	_handle_context._handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	base::unit::Nanosecond period{frequency};
	if (period < base::unit::Nanosecond{1})
	{
		throw std::invalid_argument{CODE_POS_STR + "频率过高。"};
	}

	InitializePeriod(static_cast<std::chrono::nanoseconds>(period));

	HAL_StatusTypeDef result = HAL_TIM_PWM_Init(&_handle_context._handle);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	TIM_MasterConfigTypeDef sMasterConfig{};
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	result = HAL_TIMEx_MasterConfigSynchronization(&_handle_context._handle, &sMasterConfig);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}
}

void bsp::PwmTimer3::InitializeAsDownMode(base::unit::Hz const &frequency)
{
	_handle_context._handle.Instance = TIM3;
	_handle_context._handle.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	_handle_context._handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	base::unit::Nanosecond period{frequency};
	if (period < base::unit::Nanosecond{1})
	{
		throw std::invalid_argument{CODE_POS_STR + "频率过高。"};
	}

	InitializePeriod(static_cast<std::chrono::nanoseconds>(period));

	HAL_StatusTypeDef result = HAL_TIM_PWM_Init(&_handle_context._handle);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	TIM_MasterConfigTypeDef sMasterConfig{};
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	result = HAL_TIMEx_MasterConfigSynchronization(&_handle_context._handle, &sMasterConfig);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}
}

void bsp::PwmTimer3::InitializeAsUpDownMode(base::unit::Hz const &frequency)
{
	_handle_context._handle.Instance = TIM3;
	_handle_context._handle.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
	_handle_context._handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

	base::unit::Nanosecond period{frequency};

	// 中心对齐模式先从 0 开始向上计数到重装载值，然后向下计数到 0. 如此往复，
	// 所以周期是重装载值的 2 倍。
	//
	// 所以想要设置到指定的频率，需要先将周期除以 2, 然后再计算出需要的重装载值。
	period /= 2;
	if (period < base::unit::Nanosecond{1})
	{
		throw std::invalid_argument{CODE_POS_STR + "频率过高。"};
	}

	InitializePeriod(static_cast<std::chrono::nanoseconds>(period));

	HAL_StatusTypeDef result = HAL_TIM_PWM_Init(&_handle_context._handle);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	TIM_MasterConfigTypeDef sMasterConfig{};
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	result = HAL_TIMEx_MasterConfigSynchronization(&_handle_context._handle, &sMasterConfig);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}
}

void bsp::PwmTimer3::ConfigureOutput(uint32_t channel_id,
									 base::pwm_timer::Polarity effective_polarity,
									 base::pwm_timer::Polarity idle_polarity,
									 uint32_t compare_value,
									 uint32_t dead_time)
{
	if (compare_value > UINT16_MAX)
	{
		throw std::invalid_argument{CODE_POS_STR + "比较值过大，超出范围。"};
	}

	_output_configuration.OCMode = TIM_OCMODE_PWM1;
	_output_configuration.Pulse = compare_value;

	switch (effective_polarity)
	{
	case base::pwm_timer::Polarity::Positive:
		{
			_output_configuration.OCPolarity = TIM_OCPOLARITY_HIGH;
			break;
		}
	case base::pwm_timer::Polarity::Negative:
		{
			_output_configuration.OCPolarity = TIM_OCPOLARITY_LOW;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法极性。"};
		}
	}

	switch (idle_polarity)
	{
	case base::pwm_timer::Polarity::Positive:
		{
			_output_configuration.OCIdleState = TIM_OCIDLESTATE_SET;
			break;
		}
	case base::pwm_timer::Polarity::Negative:
		{
			_output_configuration.OCIdleState = TIM_OCIDLESTATE_RESET;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法极性。"};
		}
	}

	HAL_StatusTypeDef result = HAL_TIM_PWM_ConfigChannel(&_handle_context._handle,
														 &_output_configuration,
														 bsp::channel_id_to_channel_define(channel_id));

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "配置输出失败。"};
	}
}

void bsp::PwmTimer3::Start(uint32_t channel_id)
{
	HAL_StatusTypeDef result = HAL_TIM_PWM_Start(&_handle_context._handle,
												 bsp::channel_id_to_channel_define(channel_id));

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "启动 PWM 输出失败。"};
	}
}

void bsp::PwmTimer3::StartAllChannels()
{
	HAL_StatusTypeDef result = HAL_TIM_PWM_Start(&_handle_context._handle,
												 TIM_CHANNEL_ALL);

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "启动 PWM 输出失败。"};
	}
}

void bsp::PwmTimer3::Stop(uint32_t channel_id)
{
	HAL_StatusTypeDef result = HAL_TIM_PWM_Stop(&_handle_context._handle,
												bsp::channel_id_to_channel_define(channel_id));

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "停止 PWM 输出失败。"};
	}
}

void bsp::PwmTimer3::StopAllChannels()
{
	HAL_StatusTypeDef result = HAL_TIM_PWM_Stop(&_handle_context._handle,
												TIM_CHANNEL_ALL);

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "停止 PWM 输出失败。"};
	}
}

void bsp::PwmTimer3::ChangeCompareValue(uint32_t channel_id, uint32_t value)
{
	switch (channel_id)
	{
	case 1:
		{
			TIM3->CCR1 = value;
			break;
		}
	case 2:
		{
			TIM3->CCR2 = value;
			break;
		}
	case 3:
		{
			TIM3->CCR3 = value;
			break;
		}
	case 4:
		{
			TIM3->CCR4 = value;
			break;
		}
	case 5:
		{
			TIM3->CCR5 = value;
			break;
		}
	case 6:
		{
			TIM3->CCR6 = value;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法通道 ID."};
		}
	}
}

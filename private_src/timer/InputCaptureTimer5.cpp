#include "InputCaptureTimer5.h" // IWYU pragma: keep
#include "base/embedded/interrupt/interrupt.h"
#include "base/math/FactorExtractor.h"
#include "base/math/pow.h"
#include "base/string/define.h"
#include "base/unit/Nanosecond.h"
#include "define.h"
#include "isr/timer5_isr.h"
#include <stdexcept>

/* #region 初始化辅助函数 */

void bsp::InputCaptureTimer5::InitializePeriod(std::chrono::nanoseconds const &period)
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

	// 虽然是 16 位计数器，但是要留有一定的调整的余量，所以将计数值尽量
	// 限制在 2^16 的一半，即 2^15.
	factor_extractor.ExtractConservatively(2, base::pow<uint64_t>(2, 15));
	factor_extractor.ExtractConservatively(3, base::pow<uint64_t>(2, 15));
	factor_extractor.ExtractConservatively(5, base::pow<uint64_t>(2, 15));

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

void bsp::InputCaptureTimer5::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM5_IRQn));

	set_timer5_isr(
		[this]()
		{
			HAL_TIM_IRQHandler(&_handle_context._handle);
		});

	// 先不启用中断，等到 SetPeriodElapsedCallback 函数被调用时再启用。
}

/* #endregion */

/* #region 回调 */

void bsp::InputCaptureTimer5::OnPeriodElapsedCallback()
{
	if (_on_period_elapsed_callback == nullptr)
	{
		return;
	}

	_on_period_elapsed_callback();
}

void bsp::InputCaptureTimer5::OnCaptureCompleteCallback()
{
	if (_on_capture_complete_callback == nullptr)
	{
		return;
	}

	switch (_handle_context._handle.Channel)
	{
	case HAL_TIM_ActiveChannel::HAL_TIM_ACTIVE_CHANNEL_1:
		{
			base::input_capture_timer::CaptureCompleteEventArgs args{TIM5->CCR1};
			_on_capture_complete_callback(args);
			break;
		}
	case HAL_TIM_ActiveChannel::HAL_TIM_ACTIVE_CHANNEL_2:
		{
			base::input_capture_timer::CaptureCompleteEventArgs args{TIM5->CCR2};
			_on_capture_complete_callback(args);
			break;
		}
	case HAL_TIM_ActiveChannel::HAL_TIM_ACTIVE_CHANNEL_3:
		{
			base::input_capture_timer::CaptureCompleteEventArgs args{TIM5->CCR3};
			_on_capture_complete_callback(args);
			break;
		}
	case HAL_TIM_ActiveChannel::HAL_TIM_ACTIVE_CHANNEL_4:
		{
			base::input_capture_timer::CaptureCompleteEventArgs args{TIM5->CCR4};
			_on_capture_complete_callback(args);
			break;
		}
	case HAL_TIM_ActiveChannel::HAL_TIM_ACTIVE_CHANNEL_5:
		{
			base::input_capture_timer::CaptureCompleteEventArgs args{TIM5->CCR5};
			_on_capture_complete_callback(args);
			break;
		}
	case HAL_TIM_ActiveChannel::HAL_TIM_ACTIVE_CHANNEL_6:
		{
			base::input_capture_timer::CaptureCompleteEventArgs args{TIM5->CCR6};
			_on_capture_complete_callback(args);
			break;
		}
	default:
		{
			break;
		}
	}
}

/* #endregion */

void bsp::InputCaptureTimer5::Initialize(std::chrono::nanoseconds const &period)
{
	__HAL_RCC_TIM5_CLK_ENABLE();
	_handle_context._handle.Instance = TIM5;
	_handle_context._handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	_handle_context._handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	InitializePeriod(period);

	HAL_StatusTypeDef result = HAL_TIM_IC_Init(&_handle_context._handle);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化失败。"};
	}

	// 赋值句柄中的回调函数指针
	{
		_handle_context._handle.PeriodElapsedCallback = [](TIM_HandleTypeDef *handle)
		{
			handle_context *context = reinterpret_cast<handle_context *>(handle);
			context->_self->OnPeriodElapsedCallback();
		};

		_handle_context._handle.IC_CaptureCallback = [](TIM_HandleTypeDef *handle)
		{
			handle_context *context = reinterpret_cast<handle_context *>(handle);
			context->_self->OnCaptureCompleteCallback();
		};
	}

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

void bsp::InputCaptureTimer5::ConfigureChannel(uint32_t channel_id,
											   base::input_capture_timer::CaptureEdge edge,
											   uint32_t input_prescaler)
{
	TIM_IC_InitTypeDef configuration{};
	configuration.ICSelection = TIM_ICSELECTION_DIRECTTI;
	configuration.ICFilter = 0;

	switch (edge)
	{
	case base::input_capture_timer::CaptureEdge::RisingEdge:
		{
			configuration.ICPolarity = TIM_ICPOLARITY_RISING;
			break;
		}
	case base::input_capture_timer::CaptureEdge::FallenEdge:
		{
			configuration.ICPolarity = TIM_ICPOLARITY_FALLING;
			break;
		}
	case base::input_capture_timer::CaptureEdge::BothEdge:
		{
			configuration.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法边缘。"};
		}
	}

	switch (input_prescaler)
	{
	case 1:
		{
			configuration.ICPrescaler = TIM_ICPSC_DIV1;
			break;
		}
	case 2:
		{
			configuration.ICPrescaler = TIM_ICPSC_DIV2;
			break;
		}
	case 4:
		{
			configuration.ICPrescaler = TIM_ICPSC_DIV4;
			break;
		}
	case 8:
		{
			configuration.ICPrescaler = TIM_ICPSC_DIV8;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法预分频系数。"};
		}
	}

	HAL_StatusTypeDef result = HAL_TIM_IC_ConfigChannel(&_handle_context._handle,
														&configuration,
														bsp::channel_id_to_channel_define(channel_id));

	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "配置捕获通道失败。"};
	}
}

void bsp::InputCaptureTimer5::SetPeriod(std::chrono::nanoseconds const &value)
{
	InitializePeriod(value);
	TIM_Base_SetConfig(_handle_context._handle.Instance, &_handle_context._handle.Init);
}

void bsp::InputCaptureTimer5::SetPeriodElapsedCallback(std::function<void()> const &callback)
{
	__HAL_TIM_DISABLE_IT(&_handle_context._handle, TIM_IT_UPDATE);

	if (callback == nullptr)
	{
		return;
	}

	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM5_IRQn));
	_on_period_elapsed_callback = callback;
	base::interrupt::enable_interrupt(static_cast<int32_t>(IRQn_Type::TIM5_IRQn), 5);
	__HAL_TIM_ENABLE_IT(&_handle_context._handle, TIM_IT_UPDATE);
}

void bsp::InputCaptureTimer5::SetCaptureCompleteCallback(std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> const &callback)
{
	base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM5_IRQn));
	_on_capture_complete_callback = callback;
	base::interrupt::enable_interrupt(static_cast<int32_t>(IRQn_Type::TIM5_IRQn), 5);
}

void bsp::InputCaptureTimer5::StartChannel(uint32_t channel_id)
{
	HAL_StatusTypeDef result = HAL_TIM_IC_Start_IT(&_handle_context._handle,
												   bsp::channel_id_to_channel_define(channel_id));

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "启动捕获输入通道失败。"};
	}
}

void bsp::InputCaptureTimer5::StartAllChannels()
{
	HAL_StatusTypeDef result = HAL_TIM_IC_Start_IT(&_handle_context._handle,
												   TIM_CHANNEL_ALL);

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "启动捕获输入通道失败。"};
	}
}

void bsp::InputCaptureTimer5::StopChannel(uint32_t channel_id)
{
	HAL_StatusTypeDef result = HAL_TIM_IC_Stop_IT(&_handle_context._handle,
												  bsp::channel_id_to_channel_define(channel_id));

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "停止捕获输入通道失败。"};
	}
}

void bsp::InputCaptureTimer5::StopAllChannels()
{
	HAL_StatusTypeDef result = HAL_TIM_IC_Stop_IT(&_handle_context._handle,
												  TIM_CHANNEL_ALL);

	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "停止捕获输入通道失败。"};
	}
}

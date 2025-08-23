#pragma once
#include "base/embedded/interrupt/interrupt.h"
#include "base/unit/MHz.h"
#include "base/UsageStateManager.h"
#include "base_timer_handle.h"
#include "hal.h" // IWYU pragma: keep
#include <chrono>
#include <cstdint>
#include <functional>

namespace bsp
{
	class BaseTimer3 final :
		public base::base_timer::base_timer_handle
	{
	private:
		class handle_context
		{
		public:
			handle_context(BaseTimer3 *self)
				: _self(self)
			{
			}

			TIM_HandleTypeDef _handle{};
			BaseTimer3 *_self{};
		};

		base::UsageStateManager<BaseTimer3> _usage_state_manager{};
		handle_context _handle_context{this};
		std::function<void()> _on_period_elapsed_callback;
		std::chrono::nanoseconds _period{};

		base::unit::MHz ClockSourceFrequency() const;

		void InitializePeriod(std::chrono::nanoseconds const &period);
		void InitializeInterrupt();

		void OnPeriodElapsedCallback()
		{
			if (_on_period_elapsed_callback == nullptr)
			{
				return;
			}

			try
			{
				_on_period_elapsed_callback();
			}
			catch (...)
			{
			}
		}

	public:
		///
		/// @brief 初始化定时器。
		///
		/// @param period
		///
		virtual void Initialize(std::chrono::nanoseconds const &period) override;

		///
		/// @brief 定时时间。
		///
		/// @note 初始化成功后本属性才有效。
		///
		/// @return
		///
		virtual std::chrono::nanoseconds Period() override
		{
			return _period;
		}

		///
		/// @brief 设置定时时间。
		///
		/// @note 允许运行时实时修改定时时间。
		///
		/// @note 此函数要实现为能够在定时时间到的回调中被调用，因此实现者需要避免会出现
		/// 死锁的实现方式。
		///
		/// @param period
		///
		virtual void SetPeriod(std::chrono::nanoseconds const &period) override;

		///
		/// @brief 设置定时时间到的回调。
		///
		/// @note 单片机平台直接实现为在中断中回调，不要太早使用线程和信号量进行封装。
		/// 到时候在这个基础上设计一个包装类，包装直接使用中断进行回调的定时器，使用信号量
		/// 释放被阻塞的任务去执行定时任务。
		///
		/// @warning 实现者实现时需要注意在修改回调函数的时候禁用定时器中断。
		///
		/// @param callback
		///
		virtual void SetPeriodElapsedCallback(std::function<void()> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::TIM3_IRQn));
			_on_period_elapsed_callback = callback;
			base::interrupt::enable_interrupt(static_cast<int32_t>(IRQn_Type::TIM3_IRQn), 10);
		}

		///
		/// @brief 启动定时器。
		///
		virtual void Start() override
		{
			HAL_TIM_Base_Start_IT(&_handle_context._handle);
		}

		///
		/// @brief 停止定时器。
		///
		virtual void Stop() override
		{
			HAL_TIM_Base_Stop_IT(&_handle_context._handle);
		}
	};

} // namespace bsp

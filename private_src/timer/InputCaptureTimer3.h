#pragma once
#include "base/embedded/timer/input_capture_timer_handle.h"
#include "hal.h" // IWYU pragma: keep
#include "input_capture_timer_handle.h"
#include "Timer3.h"
#include <cstdint>
#include <functional>

namespace bsp
{
	class InputCaptureTimer3 final :
		public bsp::Timer3,
		public base::input_capture_timer::input_capture_timer_handle
	{
	private:
		class handle_context
		{
		public:
			handle_context(InputCaptureTimer3 *self)
				: _self(self)
			{
			}

			TIM_HandleTypeDef _handle{};
			InputCaptureTimer3 *_self{};
		};

		handle_context _handle_context{this};
		std::function<void()> _on_period_elapsed_callback;
		std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> _on_capture_complete_callback;
		std::chrono::nanoseconds _period{};

		void InitializePeriod(std::chrono::nanoseconds const &period);
		void InitializeInterrupt();

		void OnPeriodElapsedCallback();
		void OnCaptureCompleteCallback();

	public:
		InputCaptureTimer3()
		{
			base::input_capture_timer::msp_initialize_callback(3);
		}

		virtual void Initialize(std::chrono::nanoseconds const &period) override;

		virtual uint32_t CounterPeriod() override
		{
			return _handle_context._handle.Init.Period + 1;
		}

		virtual void SetCounterPeriodPreloadValue(uint32_t value) override
		{
			if (value == 0)
			{
				value = 1;
			}
			else if (value > UINT16_MAX + 1)
			{
				value = UINT16_MAX + 1;
			}

			_handle_context._handle.Init.Period = value - 1;
			TIM3->ARR = value - 1;
		}

		virtual void ConfigureChannel(uint32_t channel_id,
									  base::input_capture_timer::CaptureEdge edge,
									  uint32_t input_prescaler) override;

		virtual std::chrono::nanoseconds Period() override
		{
			return _period;
		}

		virtual void SetPeriod(std::chrono::nanoseconds const &value) override;

		virtual void SetPeriodElapsedCallback(std::function<void()> const &callback) override;

		virtual void SetCaptureCompleteCallback(std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> const &callback) override;

		virtual void Start(uint32_t channel_id) override;

		virtual void StartAllChannels() override;

		virtual void Stop(uint32_t channel_id) override;

		virtual void StopAllChannels() override;
	};

} // namespace bsp

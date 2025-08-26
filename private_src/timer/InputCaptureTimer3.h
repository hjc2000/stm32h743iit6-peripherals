#pragma once
#include "base/embedded/timer/input_capture_timer_handle.h"
#include "hal.h" // IWYU pragma: keep
#include "input_capture_timer_handle.h"
#include "Timer3.h"
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

		virtual void initialize(std::chrono::nanoseconds const &period) override;

		virtual uint32_t counter_period() override
		{
			return _handle_context._handle.Init.Period + 1;
		}

		virtual void configure_channel(base::input_capture_timer::CaptureEdge edge,
									   uint32_t input_prescaler) override
		{
		}

		virtual std::chrono::nanoseconds period() override
		{
			return _period;
		}

		virtual void set_period(std::chrono::nanoseconds const &value) override;

		virtual void set_period_elapsed_callback(std::function<void()> const &callback) override;

		virtual void set_capture_complete_callback(std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> const &callback) override;

		virtual void start(uint32_t channel_id) override;

		virtual void start_all_channels() override;

		virtual void stop(uint32_t channel_id) override;

		virtual void stop_all_channels() override;
	};

} // namespace bsp

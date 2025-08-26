#pragma once
#include "hal.h" // IWYU pragma: keep
#include "pwm_timer_handle.h"
#include "Timer3.h"
#include <chrono>
#include <cstdint>

namespace bsp
{
	class PwmTimer3 final :
		public bsp::Timer3,
		public base::pwm_timer::pwm_timer_handle
	{
	private:
		class handle_context
		{
		public:
			handle_context(PwmTimer3 *self)
				: _self(self)
			{
			}

			TIM_HandleTypeDef _handle{};
			PwmTimer3 *_self{};
		};

		handle_context _handle_context{this};
		TIM_OC_InitTypeDef _output_configuration{};

		void InitializePeriod(std::chrono::nanoseconds const &period);

	public:
		PwmTimer3()
		{
			__HAL_RCC_TIM3_CLK_ENABLE();
			base::pwm_timer::msp_initialize_callback(3);
		}

		virtual void InitializeAsUpMode(base::unit::Hz const &frequency) override;

		virtual void InitializeAsDownMode(base::unit::Hz const &frequency) override;

		virtual void InitializeAsUpDownMode(base::unit::Hz const &frequency) override;

		virtual uint32_t CounterPeriod() const override
		{
			return _handle_context._handle.Init.Period + 1;
		}

		virtual void ConfigureOutput(uint32_t channel_id,
									 base::pwm_timer::Polarity effective_polarity,
									 base::pwm_timer::Polarity idle_polarity,
									 uint32_t compare_value,
									 uint32_t dead_time) override;

		virtual void Start(uint32_t channel_id) override;

		virtual void StartAllChannels() override;

		virtual void Stop(uint32_t channel_id) override;

		virtual void StopAllChannels() override;

		virtual void ChangeCompareValue(uint32_t channel_id, uint32_t value) override;
	};

} // namespace bsp

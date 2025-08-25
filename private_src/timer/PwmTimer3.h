#pragma once
#include "base/exception/NotSupportedException.h"
#include "base/string/define.h"
#include "base/unit/MHz.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "pwm_timer_handle.h"
#include <chrono>
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	class PwmTimer3 final :
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

		base::UsageStateManager<PwmTimer3> _usage_state_manager{};
		handle_context _handle_context{this};
		TIM_OC_InitTypeDef _output_configuration{};

		base::unit::MHz ClockSourceFrequency() const;

		void InitializePeriod(std::chrono::nanoseconds const &period);

	public:
		virtual void InitializeAsUpMode(base::unit::Hz const &frequency) override;

		virtual void InitializeAsDownMode(base::unit::Hz const &frequency) override
		{
		}

		virtual void InitializeAsUpDownMode(base::unit::Hz const &frequency) override
		{
		}

		virtual uint32_t Cycle() override
		{
			return _handle_context._handle.Init.Period;
		}

		virtual void ConfigureOutput(std::bitset<32> const &channels,
									 base::pwm_timer::Polarity effective_polarity,
									 base::pwm_timer::Polarity idle_polarity,
									 uint32_t compare_value,
									 uint32_t dead_time) override
		{
		}

		virtual void Start(base::pwm_timer::pwm_timer_handle &self) override
		{
		}

		virtual void Stop() override
		{
		}

		virtual void ChangeCompareValue(std::bitset<32> channels,
										uint32_t value) override
		{
		}
	};

} // namespace bsp

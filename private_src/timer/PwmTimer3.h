#pragma once
#include "base/exception/NotSupportedException.h"
#include "base/unit/MHz.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "pwm_timer_handle.h"

namespace bsp
{
	class PwmTimer3 final :
		public base::pwm_timer::pwm_timer_handle
	{
	private:
		base::UsageStateManager<PwmTimer3> _usage_state_manager{};
		TIM_HandleTypeDef _handle{};
		TIM_OC_InitTypeDef _output_configuration{};

		base::unit::MHz ClockSourceFrequency() const;

	public:
		virtual void InitializeAsUpMode(base::unit::Hz const &frequency,
										base::pwm_timer::Polarity effective_polarity) override
		{
			_handle.Instance = TIM3;
		}

		virtual void InitializeAsDownMode(base::unit::Hz const &frequency,
										  base::pwm_timer::Polarity effective_polarity) override
		{
		}

		virtual void InitializeAsUpDownMode(base::unit::Hz const &frequency,
											base::pwm_timer::Polarity effective_polarity) override
		{
		}

		virtual uint32_t Cycle() override
		{
			return _handle.Init.Period;
		}

		virtual uint32_t CompareValue() override
		{
			return _output_configuration.Pulse;
		}

		virtual void SetCompareValue(uint32_t value) override
		{
		}

		virtual uint32_t DeadTime() override
		{
			return 0;
		}

		virtual void SetDeadTime(uint32_t value) override
		{
			throw base::NotSupportedException{};
		}

		virtual void Start(base::pwm_timer::pwm_timer_handle &self) override
		{
		}

		virtual void Stop() override
		{
		}
	};

} // namespace bsp

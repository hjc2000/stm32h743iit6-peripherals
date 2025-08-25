#pragma once
#include "base/embedded/timer/pwm_timer_handle.h"

class base::pwm_timer::pwm_timer_handle
{
public:
	virtual void InitializeAsUpMode(base::unit::Hz const &frequency,
									base::pwm_timer::Polarity effective_polarity) = 0;

	virtual void InitializeAsDownMode(base::unit::Hz const &frequency,
									  base::pwm_timer::Polarity effective_polarity) = 0;

	virtual void InitializeAsUpDownMode(base::unit::Hz const &frequency,
										base::pwm_timer::Polarity effective_polarity) = 0;

	virtual uint32_t Cycle() = 0;

	virtual uint32_t CompareValue() = 0;

	virtual void SetCompareValue(uint32_t value) = 0;

	virtual uint32_t DeadTime() = 0;

	virtual void SetDeadTime(uint32_t value) = 0;

	virtual void Start(base::pwm_timer::pwm_timer_handle &self) = 0;

	virtual void Stop() = 0;
};

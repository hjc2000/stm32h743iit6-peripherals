#pragma once
#include "base/embedded/timer/pwm_timer_handle.h"

class base::pwm_timer::pwm_timer_handle
{
public:
	virtual void InitializeAsUpMode(base::pwm_timer::pwm_timer_handle &self,
									base::unit::Hz const &frequency,
									base::pwm_timer::Polarity effective_polarity) = 0;

	virtual void InitializeAsDownMode(base::pwm_timer::pwm_timer_handle &self,
									  base::unit::Hz const &frequency,
									  base::pwm_timer::Polarity effective_polarity) = 0;

	virtual void InitializeAsUpDownMode(base::pwm_timer::pwm_timer_handle &self,
										base::unit::Hz const &frequency,
										base::pwm_timer::Polarity effective_polarity) = 0;

	virtual uint32_t Cycle(base::pwm_timer::pwm_timer_handle const &self) = 0;

	virtual uint32_t CompareValue(base::pwm_timer::pwm_timer_handle const &self) = 0;

	virtual void SetCompareValue(base::pwm_timer::pwm_timer_handle &self, uint32_t value) = 0;

	virtual uint32_t DeadTime(base::pwm_timer::pwm_timer_handle const &self) = 0;

	virtual void SetDeadTime(base::pwm_timer::pwm_timer_handle &self, uint32_t value) = 0;

	virtual void Start(base::pwm_timer::pwm_timer_handle &self) = 0;
};

#pragma once
#include "base/embedded/timer/pwm_timer_handle.h"

class base::pwm_timer::pwm_timer_handle
{
public:
	virtual void InitializeAsUpMode(base::unit::Hz const &frequency) = 0;

	virtual void InitializeAsDownMode(base::unit::Hz const &frequency) = 0;

	virtual void InitializeAsUpDownMode(base::unit::Hz const &frequency) = 0;

	virtual uint32_t Cycle() = 0;

	virtual void ConfigureOutput(std::bitset<32> const &channels,
								 base::pwm_timer::Polarity effective_polarity,
								 base::pwm_timer::Polarity idle_polarity,
								 uint32_t compare_value,
								 uint32_t dead_time) = 0;

	virtual void Start(base::pwm_timer::pwm_timer_handle &self) = 0;

	virtual void Stop() = 0;

	virtual void ChangeCompareValue(std::bitset<32> channels,
									uint32_t value) = 0;
};

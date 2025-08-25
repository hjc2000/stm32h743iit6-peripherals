#pragma once
#include "base/embedded/timer/pwm_timer_handle.h"
#include <cstdint>

class base::pwm_timer::pwm_timer_handle
{
public:
	virtual void InitializeAsUpMode(base::unit::Hz const &frequency) = 0;

	virtual void InitializeAsDownMode(base::unit::Hz const &frequency) = 0;

	virtual void InitializeAsUpDownMode(base::unit::Hz const &frequency) = 0;

	virtual uint32_t Cycle() const = 0;

	virtual void ConfigureOutput(uint32_t channel_id,
								 base::pwm_timer::Polarity effective_polarity,
								 base::pwm_timer::Polarity idle_polarity,
								 uint32_t compare_value,
								 uint32_t dead_time) = 0;

	virtual void Start(uint32_t channel_id) = 0;

	virtual void StartAllChannels() = 0;

	virtual void ChangeCompareValue(uint32_t channel_id,
									uint32_t value) = 0;

	virtual void Stop(uint32_t channel_id) = 0;
};

#include "pwm_timer_handle.h" // IWYU pragma: keep

std::shared_ptr<base::pwm_timer::pwm_timer_handle> base::pwm_timer::open(uint32_t id);

void base::pwm_timer::initialize_as_up_mode(base::pwm_timer::pwm_timer_handle &self,
											base::unit::Hz const &frequency)
{
	self.InitializeAsUpMode(frequency);
}

void base::pwm_timer::initialize_as_down_mode(base::pwm_timer::pwm_timer_handle &self,
											  base::unit::Hz const &frequency)
{
	self.InitializeAsDownMode(frequency);
}

void base::pwm_timer::initialize_as_up_down_mode(base::pwm_timer::pwm_timer_handle &self,
												 base::unit::Hz const &frequency)
{
	self.InitializeAsUpDownMode(frequency);
}

uint32_t base::pwm_timer::cycle(base::pwm_timer::pwm_timer_handle const &self)
{
	return self.Cycle();
}

void base::pwm_timer::configure_output(base::pwm_timer::pwm_timer_handle &self,
									   uint32_t channel_id,
									   base::pwm_timer::Polarity effective_polarity,
									   base::pwm_timer::Polarity idle_polarity,
									   uint32_t compare_value,
									   uint32_t dead_time)
{
	self.ConfigureOutput(channel_id,
						 effective_polarity,
						 idle_polarity,
						 compare_value,
						 dead_time);
}

void base::pwm_timer::start(base::pwm_timer::pwm_timer_handle &self, uint32_t channel_id)
{
	self.Start(channel_id);
}

void base::pwm_timer::change_compare_value(base::pwm_timer::pwm_timer_handle &self,
										   uint32_t channel_id,
										   uint32_t value)
{
	self.ChangeCompareValue(channel_id, value);
}

void base::pwm_timer::stop(base::pwm_timer::pwm_timer_handle &self, uint32_t channel_id)
{
	self.Stop(channel_id);
}

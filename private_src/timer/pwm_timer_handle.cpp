#include "pwm_timer_handle.h" // IWYU pragma: keep

std::shared_ptr<base::pwm_timer::pwm_timer_handle> base::pwm_timer::open(uint32_t id);

void base::pwm_timer::initialize_as_up_mode(base::pwm_timer::pwm_timer_handle &self,
											base::unit::Hz const &frequency,
											base::pwm_timer::Polarity effective_polarity);

void base::pwm_timer::initialize_as_down_mode(base::pwm_timer::pwm_timer_handle &self,
											  base::unit::Hz const &frequency,
											  base::pwm_timer::Polarity effective_polarity);

void base::pwm_timer::initialize_as_up_down_mode(base::pwm_timer::pwm_timer_handle &self,
												 base::unit::Hz const &frequency,
												 base::pwm_timer::Polarity effective_polarity);

uint32_t base::pwm_timer::cycle(base::pwm_timer::pwm_timer_handle const &self);

uint32_t base::pwm_timer::compare_value(base::pwm_timer::pwm_timer_handle const &self);

void base::pwm_timer::set_compare_value(base::pwm_timer::pwm_timer_handle &self, uint32_t value);

uint32_t base::pwm_timer::dead_time(base::pwm_timer::pwm_timer_handle const &self);

void base::pwm_timer::set_dead_time(base::pwm_timer::pwm_timer_handle &self, uint32_t value);

void base::pwm_timer::start(base::pwm_timer::pwm_timer_handle &self);

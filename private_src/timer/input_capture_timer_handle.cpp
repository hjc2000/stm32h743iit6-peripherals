#include "input_capture_timer_handle.h" // IWYU pragma: keep

std::shared_ptr<base::input_capture_timer::input_capture_timer_handle> base::input_capture_timer::open(uint32_t id);

void base::input_capture_timer::initialize(base::input_capture_timer::input_capture_timer_handle &self,
										   std::chrono::nanoseconds const &period)
{
	self.initialize(period);
}

uint32_t base::input_capture_timer::counter_period(base::input_capture_timer::input_capture_timer_handle &self)
{
	return self.counter_period();
}

void base::input_capture_timer::configure_channel(base::input_capture_timer::input_capture_timer_handle &self,
												  base::input_capture_timer::CaptureEdge edge,
												  uint32_t input_prescaler)
{
	self.configure_channel(edge, input_prescaler);
}

std::chrono::nanoseconds base::input_capture_timer::period(base::input_capture_timer::input_capture_timer_handle &self)
{
	return self.period();
}

void base::input_capture_timer::set_period(base::input_capture_timer::input_capture_timer_handle &self,
										   std::chrono::nanoseconds const &value)
{
	self.set_period(value);
}

void base::input_capture_timer::set_period_elapsed_callback(base::input_capture_timer::input_capture_timer_handle &self,
															std::function<void()> const &callback);

void base::input_capture_timer::set_capture_complete_callback(base::input_capture_timer::input_capture_timer_handle &self,
															  std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> const &callback);

void base::input_capture_timer::start(base::input_capture_timer::input_capture_timer_handle &self, uint32_t channel_id);

void base::input_capture_timer::start_all_channels(base::input_capture_timer::input_capture_timer_handle &self);

void base::input_capture_timer::stop(base::input_capture_timer::input_capture_timer_handle &self, uint32_t channel_id);

void base::input_capture_timer::stop_all_channels(base::input_capture_timer::input_capture_timer_handle &self);

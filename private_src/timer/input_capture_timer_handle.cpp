#include "input_capture_timer_handle.h" // IWYU pragma: keep
#include "base/string/define.h"
#include "InputCaptureTimer3.h"
#include <stdexcept>

std::shared_ptr<base::input_capture_timer::input_capture_timer_handle> base::input_capture_timer::open(uint32_t id)
{
	switch (id)
	{
	case 3:
		{
			return std::shared_ptr<base::input_capture_timer::input_capture_timer_handle>{new bsp::InputCaptureTimer3{}};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法 ID."};
		}
	}
}

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
												  uint32_t channel_id,
												  base::input_capture_timer::CaptureEdge edge,
												  uint32_t input_prescaler)
{
	self.configure_channel(channel_id, edge, input_prescaler);
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
															std::function<void()> const &callback)
{
	self.set_period_elapsed_callback(callback);
}

void base::input_capture_timer::set_capture_complete_callback(base::input_capture_timer::input_capture_timer_handle &self,
															  std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> const &callback)
{
	self.set_capture_complete_callback(callback);
}

void base::input_capture_timer::start(base::input_capture_timer::input_capture_timer_handle &self, uint32_t channel_id)
{
	self.start(channel_id);
}

void base::input_capture_timer::start_all_channels(base::input_capture_timer::input_capture_timer_handle &self)
{
	self.start_all_channels();
}

void base::input_capture_timer::stop(base::input_capture_timer::input_capture_timer_handle &self, uint32_t channel_id)
{
	self.stop(channel_id);
}

void base::input_capture_timer::stop_all_channels(base::input_capture_timer::input_capture_timer_handle &self)
{
	self.stop_all_channels();
}

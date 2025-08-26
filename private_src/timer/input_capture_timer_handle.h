#pragma once
#include "base/embedded/timer/input_capture_timer_handle.h"

class base::input_capture_timer::input_capture_timer_handle
{
public:
	virtual void initialize(std::chrono::nanoseconds const &period) = 0;

	virtual uint32_t counter_period() = 0;

	virtual void configure_channel(base::input_capture_timer::CaptureEdge edge,
								   uint32_t input_prescaler) = 0;

	virtual std::chrono::nanoseconds period() = 0;

	virtual void set_period(std::chrono::nanoseconds const &period) = 0;

	virtual void set_period_elapsed_callback(std::function<void()> const &callback) = 0;

	virtual void set_capture_complete_callback(std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> const &callback) = 0;

	virtual void start(uint32_t channel_id) = 0;

	virtual void start_all_channels() = 0;

	virtual void stop(uint32_t channel_id) = 0;

	virtual void stop_all_channels() = 0;
};

#pragma once
#include "base/embedded/timer/input_capture_timer_handle.h"

class base::input_capture_timer::input_capture_timer_handle
{
public:
	virtual void Initialize(std::chrono::nanoseconds const &period) = 0;

	virtual uint32_t CounterPeriod() = 0;

	virtual void SetCounterPeriodPreloadValue(uint32_t value) = 0;

	virtual void ConfigureChannel(uint32_t channel_id,
								  base::input_capture_timer::CaptureEdge edge,
								  uint32_t input_prescaler) = 0;

	virtual std::chrono::nanoseconds Period() = 0;

	virtual void SetPeriod(std::chrono::nanoseconds const &value) = 0;

	virtual void SetPeriodElapsedCallback(std::function<void()> const &callback) = 0;

	virtual void SetCaptureCompleteCallback(std::function<void(base::input_capture_timer::CaptureCompleteEventArgs const &)> const &callback) = 0;

	virtual void Start() = 0;

	virtual void Stop() = 0;

	virtual void StartChannel(uint32_t channel_id) = 0;

	virtual void StartAllChannels() = 0;

	virtual void StopChannel(uint32_t channel_id) = 0;

	virtual void StopAllChannels() = 0;
};

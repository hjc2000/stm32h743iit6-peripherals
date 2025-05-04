#pragma once
#include "base/embedded/clock/clock_source_handle.h"
#include "base/unit/MHz.h"
#include <string>

class base::clock::clock_source_handle
{
public:
	virtual base::MHz Frequency() const = 0;
	virtual base::MHz Frequency(std::string const &output_channel_name) const = 0;

	virtual void Configure() = 0;

	virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) = 0;

	virtual void ConfigureAsBypassMode(base::MHz const &bypass_input_frequency) = 0;

	virtual void TurnOff() = 0;
};

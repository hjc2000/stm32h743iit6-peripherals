#pragma once
#include "base/embedded/clock/clock_source_handle.h"
#include "base/unit/MHz.h"

class base::clock::clock_source_handle
{
public:
	virtual base::MHz Frequency() const = 0;

	virtual void Configure() = 0;

	virtual void Configure(std::map<uint32_t, uint32_t> const &channel_factor_map) = 0;
};

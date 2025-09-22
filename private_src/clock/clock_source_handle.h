#pragma once
#include "base/embedded/clock/clock_source_handle.h"
#include "base/exception/NotSupportedException.h"
#include "base/unit/MHz.h"
#include <string>

class base::clock::clock_source_handle
{
public:
	/* #region Frequency */

	virtual base::unit::MHz Frequency() const
	{
		throw base::NotSupportedException{};
	}

	virtual base::unit::MHz Frequency(std::string const &output_channel_name) const
	{
		throw base::NotSupportedException{};
	}

	/* #endregion */

	/* #region Configure */

	virtual void Configure()
	{
		throw base::NotSupportedException{};
	}

	virtual void Configure(uint32_t input_divider)
	{
		throw base::NotSupportedException{};
	}

	virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map)
	{
		throw base::NotSupportedException{};
	}

	virtual void Configure(std::string const &input_channel_name,
						   uint32_t input_divider)
	{
		throw base::NotSupportedException{};
	}

	virtual void Configure(std::string const &input_channel_name,
						   std::map<std::string, uint32_t> const &channel_factor_map)
	{
		throw base::NotSupportedException{};
	}

	/* #endregion */

	virtual void ConfigureAsBypassMode(base::unit::MHz const &bypass_input_frequency)
	{
		throw base::NotSupportedException{};
	}

	virtual void TurnOff()
	{
		throw base::NotSupportedException{};
	}
};

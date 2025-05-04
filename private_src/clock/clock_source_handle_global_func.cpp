#include "base/string/define.h"
#include "clock_source_handle.h"
#include "HclkClockSignal.h"
#include "HseClockSource.h"
#include "PllClockSource.h"
#include <memory>
#include <stdexcept>

std::shared_ptr<base::clock::clock_source_handle> base::clock::open(std::string const &name)
{
	if (name == "hse")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::HseClockSource{}};
	}
	else if (name == "pll")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::PllClockSource{}};
	}
	else if (name == "hclk")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::HclkClockSignal{}};
	}

	throw std::invalid_argument{CODE_POS_STR + "没有时钟源名为：" + name};
}

base::MHz base::clock::frequency(clock_source_handle &h)
{
	return h.Frequency();
}

/* #region configure */

void base::clock::configure(clock_source_handle &h)
{
	h.Configure();
}

void base::clock::configure(clock_source_handle &h,
							std::map<std::string, uint32_t> const &channel_factor_map)
{
	h.Configure(channel_factor_map);
}

void base::clock::configure(clock_source_handle &h,
							std::string const &input_channel_name,
							std::map<std::string, uint32_t> const &channel_factor_map)
{
	h.Configure(input_channel_name, channel_factor_map);
}

/* #endregion */

void base::clock::configure_as_bypass_mode(clock_source_handle &h,
										   base::MHz const &bypass_input_frequency)
{
	h.ConfigureAsBypassMode(bypass_input_frequency);
}

void base::clock::turn_off(clock_source_handle &h)
{
	h.TurnOff();
}

#include "clock_source_handle.h"
#include "base/string/define.h"
#include "D1Pclk1ClockSignal.h"
#include "D3Pclk1ClockSignal.h"
#include "FmcClock.h"
#include "HclkClockSignal.h"
#include "HseClockSource.h"
#include "Pclk1ClockSignal.h"
#include "Pclk2ClockSignal.h"
#include "PllClockSource.h"
#include "PllClockSource2.h"
#include "PllClockSource3.h"
#include "SysclkClockSignal.h"
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
	else if (name == "pll2")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::PllClockSource2{}};
	}
	else if (name == "pll3")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::PllClockSource3{}};
	}
	else if (name == "sysclk")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::SysclkClockSignal{}};
	}
	else if (name == "hclk")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::HclkClockSignal{}};
	}
	else if (name == "pclk1")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::Pclk1ClockSignal{}};
	}
	else if (name == "pclk2")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::Pclk2ClockSignal{}};
	}
	else if (name == "d1pclk1")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::D1Pclk1ClockSignal{}};
	}
	else if (name == "d3pclk1")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::D3Pclk1ClockSignal{}};
	}
	else if (name == "fmc")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::FmcClock{}};
	}

	throw std::invalid_argument{CODE_POS_STR + "没有时钟源名为：" + name};
}

/* #region frequency */

base::unit::MHz base::clock::frequency(base::clock::clock_source_handle &self)
{
	return self.Frequency();
}

base::unit::MHz base::clock::frequency(base::clock::clock_source_handle &self,
									   std::string const &output_channel_name)
{
	return self.Frequency(output_channel_name);
}

/* #endregion */

/* #region configure */

void base::clock::configure(base::clock::clock_source_handle &self)
{
	self.Configure();
}

void base::clock::configure(base::clock::clock_source_handle &self, uint32_t input_divider)
{
	self.Configure(input_divider);
}

void base::clock::configure(base::clock::clock_source_handle &self,
							std::map<std::string, uint32_t> const &channel_factor_map)
{
	self.Configure(channel_factor_map);
}

void base::clock::configure(base::clock::clock_source_handle &self,
							std::string const &input_channel_name)
{
	self.Configure(input_channel_name);
}

void base::clock::configure(base::clock::clock_source_handle &self,
							std::string const &input_channel_name,
							uint32_t input_divider)
{
	self.Configure(input_channel_name, input_divider);
}

void base::clock::configure(base::clock::clock_source_handle &self,
							std::string const &input_channel_name,
							std::map<std::string, uint32_t> const &channel_factor_map)
{
	self.Configure(input_channel_name, channel_factor_map);
}

/* #endregion */

void base::clock::configure_as_bypass_mode(base::clock::clock_source_handle &self,
										   base::unit::MHz const &bypass_input_frequency)
{
	self.ConfigureAsBypassMode(bypass_input_frequency);
}

void base::clock::turn_off(base::clock::clock_source_handle &self)
{
	self.TurnOff();
}

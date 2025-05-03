#include "base/string/define.h"
#include "clock_source_handle.h"
#include "HseClockSource.h"
#include <memory>
#include <stdexcept>

std::shared_ptr<base::clock::clock_source_handle> base::clock::open(std::string const &name)
{
	if (name == "hse")
	{
		return std::shared_ptr<base::clock::clock_source_handle>{new bsp::HseClockSource{}};
	}

	throw std::invalid_argument{CODE_POS_STR + "没有时钟源名为：" + name};
}

base::MHz base::clock::frequency(clock_source_handle &h)
{
	return h.Frequency();
}

void base::clock::configure(clock_source_handle &h)
{
	h.Configure();
}

void base::clock::configure(clock_source_handle &h,
							std::map<std::string, uint32_t> const &channel_factor_map)
{
	h.Configure(channel_factor_map);
}

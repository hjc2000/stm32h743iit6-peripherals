#include "FmcClock.h" // IWYU pragma: keep
#include "base/embedded/clock/ClockSource.h"
#include "base/string/define.h"
#include <stdexcept>

base::unit::MHz bsp::FmcClock::Frequency() const
{
	if (!_configured)
	{
		throw std::runtime_error{CODE_POS_STR + "需要先通过本类配置后才能查询频率。"};
	}

	switch (_input_channel)
	{
	case InputChannel::HCLK3:
		{
			base::clock::ClockSource clock_source{"hclk"};
			base::unit::MHz clock_source_freq = clock_source.Frequency();
			return clock_source_freq;
		}
	case InputChannel::PLL1_Q:
	case InputChannel::PLL2_R:
	case InputChannel::PER_CK:
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "无效的时钟源输入通道。"};
		}
	}
}

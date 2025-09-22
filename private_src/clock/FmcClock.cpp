#include "FmcClock.h" // IWYU pragma: keep
#include "base/embedded/clock/ClockSource.h"

base::unit::MHz bsp::FmcClock::Frequency() const
{
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

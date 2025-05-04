#include "base/define.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/clock.h>
#include <D1Pclk1ClockSignal.h>
#include <D3Pclk1ClockSignal.h>
#include <HclkClockSignal.h>
#include <Pclk1ClockSignal.h>
#include <Pclk2ClockSignal.h>
#include <SysclkClockSignal.h>

namespace
{
	class DictionaryProvider
	{
	public:
		DictionaryProvider()
		{
			_dic.Add(bsp::D1Pclk1ClockSignal::Instance().Name(), &bsp::D1Pclk1ClockSignal::Instance());
			_dic.Add(bsp::D3Pclk1ClockSignal::Instance().Name(), &bsp::D3Pclk1ClockSignal::Instance());
		}

		base::Dictionary<std::string, bsp::IClockSignal *> _dic{};
	};

} // namespace

PREINIT(bsp::di::clock::ClockSignalCollection);

base::IDictionary<std::string, bsp::IClockSignal *> const &bsp::di::clock::ClockSignalCollection()
{
	static DictionaryProvider o{};
	return o._dic;
}

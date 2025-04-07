#include <base/container/Dictionary.h>
#include <bsp-interface/di/clock.h>
#include <HseClockSource.h>
#include <PllClockSource.h>

namespace
{
	class DictionaryProvider
	{
	public:
		DictionaryProvider()
		{
			_dic.Add(bsp::HseClockSource::Instance().Name(), &bsp::HseClockSource::Instance());
			_dic.Add(bsp::PllClockSource::Instance().Name(), &bsp::PllClockSource::Instance());
		}

		base::Dictionary<std::string, bsp::IClockSource *> _dic{};
	};

	class Init
	{
	public:
		Init()
		{
			bsp::di::clock::ClockSourceCollection();
		}
	};

	Init volatile _dic_hjc_init{};

} // namespace

base::IDictionary<std::string, bsp::IClockSource *> const &bsp::di::clock::ClockSourceCollection()
{
	static DictionaryProvider o;
	return o._dic;
}

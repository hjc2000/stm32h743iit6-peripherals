#include "base/container/Dictionary.h"
#include "base/define.h"

/* #region PB */

#include "PB/GpioPinPB12.h"
#include "PB/GpioPinPB5.h"

/* #endregion */

namespace
{
	class DictionaryProvider
	{
	private:
		void AddPin(bsp::IGpioPin &pin)
		{
			_dic.Add(pin.PinName(), &pin);
		}

	public:
		DictionaryProvider()
		{
			/* #region PB */

			AddPin(bsp::GpioPinPB5::Instance());
			AddPin(bsp::GpioPinPB12::Instance());

			/* #endregion */
		}

		base::Dictionary<std::string, bsp::IGpioPin *> _dic{};
	};

} // namespace

PREINIT(DI_GpioPinCollection);

base::IDictionary<std::string, bsp::IGpioPin *> const &DI_GpioPinCollection()
{
	static DictionaryProvider o{};
	return o._dic;
}

#include "base/container/Dictionary.h"
#include "base/define.h"

/* #region PA */

#include "PA/GpioPinPA0.h"
#include "PA/GpioPinPA1.h"

/* #endregion */

/* #region PB */

#include "PB/GpioPinPB12.h"
#include "PB/GpioPinPB5.h"

/* #endregion */

/* #region PC */

#include "PC/GpioPinPC1.h"
#include "PC/GpioPinPC5.h"

/* #endregion */

/* #region PD */

#include "PD/GpioPinPD11.h"
#include "PD/GpioPinPD4.h"
#include "PD/GpioPinPD5.h"

/* #endregion */

/* #region PE */

#include "PE/GpioPinPE2.h"
#include "PE/GpioPinPE3.h"
#include "PE/GpioPinPE4.h"
#include "PE/GpioPinPE5.h"

/* #endregion */

/* #region PG */

#include "PG/GpioPinPG12.h"

/* #endregion */

/* #region PH */

#include "PH/GpioPinPH2.h"
#include "PH/GpioPinPH3.h"
#include "PH/GpioPinPH4.h"
#include "PH/GpioPinPH5.h"

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
			/* #region PA */

			AddPin(bsp::GpioPinPA0::Instance());
			AddPin(bsp::GpioPinPA1::Instance());

			/* #endregion */

			/* #region PB */

			AddPin(bsp::GpioPinPB5::Instance());
			AddPin(bsp::GpioPinPB12::Instance());

			/* #endregion */

			/* #region PC */

			AddPin(bsp::GpioPinPC1::Instance());
			AddPin(bsp::GpioPinPC5::Instance());

			/* #endregion */

			/* #region PD */

			AddPin(bsp::GpioPinPD4::Instance());
			AddPin(bsp::GpioPinPD5::Instance());
			AddPin(bsp::GpioPinPD11::Instance());

			/* #endregion */

			/* #region PE */

			AddPin(bsp::GpioPinPE2::Instance());
			AddPin(bsp::GpioPinPE3::Instance());
			AddPin(bsp::GpioPinPE4::Instance());
			AddPin(bsp::GpioPinPE5::Instance());

			/* #endregion */

			/* #region PG */

			AddPin(bsp::GpioPinPG12::Instance());

			/* #endregion */

			/* #region PH */

			AddPin(bsp::GpioPinPH2::Instance());
			AddPin(bsp::GpioPinPH3::Instance());
			AddPin(bsp::GpioPinPH4::Instance());
			AddPin(bsp::GpioPinPH5::Instance());

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

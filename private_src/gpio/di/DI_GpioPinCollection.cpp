#include "base/container/Dictionary.h"
#include "base/define.h"

/* #region PA */

#include "PA/GpioPinPA0.h"
#include "PA/GpioPinPA1.h"

/* #endregion */

/* #region PB */

#include "PB/GpioPinPB0.h"
#include "PB/GpioPinPB1.h"
#include "PB/GpioPinPB12.h"
#include "PB/GpioPinPB5.h"

/* #endregion */

/* #region PC */

#include "PC/GpioPinPC1.h"
#include "PC/GpioPinPC5.h"

/* #endregion */

/* #region PD */

#include "PD/GpioPinPD11.h"
#include "PD/GpioPinPD14.h"
#include "PD/GpioPinPD15.h"
#include "PD/GpioPinPD4.h"
#include "PD/GpioPinPD5.h"

/* #endregion */

/* #region PE */

#include "PE/GpioPinPE0.h"
#include "PE/GpioPinPE1.h"
#include "PE/GpioPinPE10.h"
#include "PE/GpioPinPE11.h"
#include "PE/GpioPinPE12.h"
#include "PE/GpioPinPE13.h"
#include "PE/GpioPinPE14.h"
#include "PE/GpioPinPE15.h"
#include "PE/GpioPinPE2.h"
#include "PE/GpioPinPE3.h"
#include "PE/GpioPinPE4.h"
#include "PE/GpioPinPE5.h"
#include "PE/GpioPinPE7.h"
#include "PE/GpioPinPE8.h"
#include "PE/GpioPinPE9.h"

/* #endregion */

/* #region PF */

#include "PF/GpioPinPF0.h"
#include "PF/GpioPinPF1.h"
#include "PF/GpioPinPF11.h"
#include "PF/GpioPinPF12.h"
#include "PF/GpioPinPF13.h"
#include "PF/GpioPinPF14.h"
#include "PF/GpioPinPF15.h"
#include "PF/GpioPinPF2.h"
#include "PF/GpioPinPF3.h"
#include "PF/GpioPinPF4.h"
#include "PF/GpioPinPF5.h"

/* #endregion */

/* #region PG */

#include "PG/GpioPinPG0.h"
#include "PG/GpioPinPG1.h"
#include "PG/GpioPinPG12.h"
#include "PG/GpioPinPG15.h"
#include "PG/GpioPinPG2.h"
#include "PG/GpioPinPG4.h"
#include "PG/GpioPinPG5.h"
#include "PG/GpioPinPG8.h"

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

			AddPin(bsp::GpioPinPB0::Instance());
			AddPin(bsp::GpioPinPB1::Instance());
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
			AddPin(bsp::GpioPinPD14::Instance());
			AddPin(bsp::GpioPinPD15::Instance());

			/* #endregion */

			/* #region PE */

			AddPin(bsp::GpioPinPE0::Instance());
			AddPin(bsp::GpioPinPE1::Instance());
			AddPin(bsp::GpioPinPE2::Instance());
			AddPin(bsp::GpioPinPE3::Instance());
			AddPin(bsp::GpioPinPE4::Instance());
			AddPin(bsp::GpioPinPE5::Instance());
			AddPin(bsp::GpioPinPE7::Instance());
			AddPin(bsp::GpioPinPE8::Instance());
			AddPin(bsp::GpioPinPE9::Instance());
			AddPin(bsp::GpioPinPE10::Instance());
			AddPin(bsp::GpioPinPE11::Instance());
			AddPin(bsp::GpioPinPE12::Instance());
			AddPin(bsp::GpioPinPE13::Instance());
			AddPin(bsp::GpioPinPE14::Instance());
			AddPin(bsp::GpioPinPE15::Instance());

			/* #endregion */

			/* #region PF */

			AddPin(bsp::GpioPinPF0::Instance());
			AddPin(bsp::GpioPinPF1::Instance());
			AddPin(bsp::GpioPinPF2::Instance());
			AddPin(bsp::GpioPinPF3::Instance());
			AddPin(bsp::GpioPinPF4::Instance());
			AddPin(bsp::GpioPinPF5::Instance());
			AddPin(bsp::GpioPinPF11::Instance());
			AddPin(bsp::GpioPinPF12::Instance());
			AddPin(bsp::GpioPinPF13::Instance());
			AddPin(bsp::GpioPinPF14::Instance());
			AddPin(bsp::GpioPinPF15::Instance());

			/* #endregion */

			/* #region PG */

			AddPin(bsp::GpioPinPG0::Instance());
			AddPin(bsp::GpioPinPG1::Instance());
			AddPin(bsp::GpioPinPG2::Instance());
			AddPin(bsp::GpioPinPG4::Instance());
			AddPin(bsp::GpioPinPG5::Instance());
			AddPin(bsp::GpioPinPG8::Instance());
			AddPin(bsp::GpioPinPG12::Instance());
			AddPin(bsp::GpioPinPG15::Instance());

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

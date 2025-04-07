#include "base/container/Dictionary.h"
#include "Serial.h"

base::serial::ISerial &base::serial::MainSerial()
{
	return bsp::Serial::Instance();
}

namespace
{
	class DictionaryProvider
	{
	private:
		void Add(base::serial::ISerial *o)
		{
			_dic.Add(o->Name(), o);
		}

	public:
		DictionaryProvider()
		{
			Add(&bsp::Serial::Instance());
		}

		base::Dictionary<std::string, base::serial::ISerial *> _dic{};
	};

} // namespace

PREINIT(base::serial::SerialCollection);

base::IDictionary<std::string, base::serial::ISerial *> const &base::serial::SerialCollection()
{
	static DictionaryProvider o{};
	return o._dic;
}

#include "base/container/Dictionary.h"
#include "serial_handle.h"

base::serial::ISerial &base::serial::MainSerial()
{
	return base::serial::serial_handle::Instance();
}

namespace
{
	class DictionaryProvider
	{
	private:
		void Add(base::AutoPtr<base::serial::ISerial> o)
		{
			_dic.Add("serial", o);
		}

	public:
		DictionaryProvider()
		{
			Add(&base::serial::serial_handle::Instance());
		}

		base::Dictionary<std::string, base::AutoPtr<base::serial::ISerial>> _dic{};
	};

} // namespace

PREINIT(base::serial::SerialCollection);

base::IDictionary<std::string, base::AutoPtr<base::serial::ISerial>> const &base::serial::SerialCollection()
{
	static DictionaryProvider o{};
	return o._dic;
}

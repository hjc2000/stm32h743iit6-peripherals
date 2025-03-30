#include "base/peripheral/ISerial.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <Serial.h>

namespace
{
	class Collection
	{
	private:
		void Add(base::serial::ISerial *o)
		{
			_dic.Add(o->Name(), o);
		}

	public:
		Collection()
		{
			Add(&bsp::Serial::Instance());
		}

		base::Dictionary<std::string, base::serial::ISerial *> _dic{};
	};

	class Getter :
		public bsp::TaskSingletonGetter<Collection>
	{
	public:
		std::unique_ptr<Collection> Create() override
		{
			return std::unique_ptr<Collection>{new Collection{}};
		}
	};
} // namespace

base::IDictionary<std::string, base::serial::ISerial *> const &base::serial::SerialCollection()
{
	Getter g;
	return g.Instance()._dic;
}

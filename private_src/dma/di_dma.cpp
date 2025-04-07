#include "base/define.h"
#include "Dma1Stream0.h"
#include "Dma1Stream1.h"
#include <base/container/Dictionary.h>
#include <bsp-interface/di/dma.h>
#include <bsp-interface/di/interrupt.h>

namespace
{
	class DictionaryProvider
	{
	private:
		void Add(bsp::dma::IDmaChannel *o)
		{
			_dic.Add(o->Name(), o);
		}

	public:
		DictionaryProvider()
		{
			Add(&bsp::Dma1Stream0::Instance());
			Add(&bsp::Dma1Stream1::Instance());
		}

		base::Dictionary<std::string, bsp::dma::IDmaChannel *> _dic;
	};

} // namespace

PREINIT(DI_DmaChannelCollection);

base::IDictionary<std::string, bsp::dma::IDmaChannel *> const &DI_DmaChannelCollection()
{
	static DictionaryProvider o{};
	return o._dic;
}

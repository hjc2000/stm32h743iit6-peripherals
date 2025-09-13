// #include "base/define.h"
// #include <base/container/Dictionary.h>
// #include <bsp-interface/di/flash.h>
// #include <Flash.h>

// namespace
// {
// 	class DictionaryProvider
// 	{
// 	private:
// 		void Add(bsp::IFlash *flash)
// 		{
// 			_dic.Add(flash->Name(), flash);
// 		}

// 	public:
// 		DictionaryProvider()
// 		{
// 			Add(&bsp::Flash::Instance());
// 		}

// 		base::Dictionary<std::string, bsp::IFlash *> _dic;
// 	};

// } // namespace

// base::IDictionary<std::string, bsp::IFlash *> const &DI_FlashCollection()
// {
// 	static DictionaryProvider o{};
// 	return o._dic;
// }

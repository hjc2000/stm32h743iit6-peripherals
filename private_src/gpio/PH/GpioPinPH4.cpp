#include "GpioPinPH4.h"
#include <hal.h>

namespace
{
	class Init
	{
	public:
		Init()
		{
			bsp::GpioPinPH4::Instance();
		}
	};

	Init volatile _ph4_hjc_init{};

} // namespace

bsp::GpioPinPH4 &bsp::GpioPinPH4::Instance()
{
	static GpioPinPH4 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPH4::Port()
{
	return GPIOH;
}

uint32_t bsp::GpioPinPH4::Pin()
{
	return GPIO_PIN_4;
}

std::string bsp::GpioPinPH4::PinName() const
{
	return "PH4";
}

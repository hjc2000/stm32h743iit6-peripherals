#include "GpioPinPH5.h"
#include <hal.h>

namespace
{
	class Init
	{
	public:
		Init()
		{
			bsp::GpioPinPH5::Instance();
		}
	};

	Init volatile _ph5_hjc_init{};

} // namespace

bsp::GpioPinPH5 &bsp::GpioPinPH5::Instance()
{
	static GpioPinPH5 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPH5::Port()
{
	return GPIOH;
}

uint32_t bsp::GpioPinPH5::Pin()
{
	return GPIO_PIN_5;
}

std::string bsp::GpioPinPH5::PinName() const
{
	return "PH5";
}

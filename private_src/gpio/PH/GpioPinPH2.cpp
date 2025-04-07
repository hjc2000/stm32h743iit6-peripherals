#include "GpioPinPH2.h"
#include <hal.h>

namespace
{
	class Init
	{
	public:
		Init()
		{
			bsp::GpioPinPH2::Instance();
		}
	};

	Init volatile _ph2_hjc_init{};

} // namespace

bsp::GpioPinPH2 &bsp::GpioPinPH2::Instance()
{
	static GpioPinPH2 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPH2::Port()
{
	return GPIOH;
}

uint32_t bsp::GpioPinPH2::Pin()
{
	return GPIO_PIN_2;
}

std::string bsp::GpioPinPH2::PinName() const
{
	return "PH2";
}

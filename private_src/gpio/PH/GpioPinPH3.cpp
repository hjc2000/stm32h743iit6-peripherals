#include "GpioPinPH3.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPH3::Instance);

bsp::GpioPinPH3 &bsp::GpioPinPH3::Instance()
{
	static GpioPinPH3 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPH3::Port()
{
	return GPIOH;
}

uint32_t bsp::GpioPinPH3::Pin()
{
	return GPIO_PIN_3;
}

std::string bsp::GpioPinPH3::PinName() const
{
	return "PH3";
}

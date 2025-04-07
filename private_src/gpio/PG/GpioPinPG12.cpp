#include "GpioPinPG12.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPG12::Instance);

bsp::GpioPinPG12 &bsp::GpioPinPG12::Instance()
{
	static GpioPinPG12 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPG12::Port()
{
	return GPIOG;
}

uint32_t bsp::GpioPinPG12::Pin()
{
	return GPIO_PIN_12;
}

std::string bsp::GpioPinPG12::PinName() const
{
	return "PG12";
}

#include "GpioPinPA0.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPA0::Instance);

bsp::GpioPinPA0 &bsp::GpioPinPA0::Instance()
{
	static GpioPinPA0 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPA0::Port()
{
	return GPIOA;
}

uint32_t bsp::GpioPinPA0::Pin()
{
	return GPIO_PIN_0;
}

std::string bsp::GpioPinPA0::PinName() const
{
	return "PA0";
}

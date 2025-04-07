#include "GpioPinPC5.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPC5::Instance);

bsp::GpioPinPC5 &bsp::GpioPinPC5::Instance()
{
	static GpioPinPC5 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPC5::Port()
{
	return GPIOC;
}

uint32_t bsp::GpioPinPC5::Pin()
{
	return GPIO_PIN_5;
}

std::string bsp::GpioPinPC5::PinName() const
{
	return "PC5";
}

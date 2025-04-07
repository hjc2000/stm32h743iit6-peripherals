#include "GpioPinPD4.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPD4::Instance);

bsp::GpioPinPD4 &bsp::GpioPinPD4::Instance()
{
	static GpioPinPD4 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPD4::Port()
{
	return GPIOD;
}

uint32_t bsp::GpioPinPD4::Pin()
{
	return GPIO_PIN_4;
}

std::string bsp::GpioPinPD4::PinName() const
{
	return "PD4";
}

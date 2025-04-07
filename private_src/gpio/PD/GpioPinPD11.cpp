#include "GpioPinPD11.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPD11::Instance)

bsp::GpioPinPD11 &bsp::GpioPinPD11::Instance()
{
	static GpioPinPD11 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPD11::Port()
{
	return GPIOD;
}

uint32_t bsp::GpioPinPD11::Pin()
{
	return GPIO_PIN_11;
}

std::string bsp::GpioPinPD11::PinName() const
{
	return "PD11";
}

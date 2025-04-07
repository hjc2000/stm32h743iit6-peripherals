#include "GpioPinPD5.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPD5::Instance);

bsp::GpioPinPD5 &bsp::GpioPinPD5::Instance()
{
	static GpioPinPD5 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPD5::Port()
{
	return GPIOD;
}

uint32_t bsp::GpioPinPD5::Pin()
{
	return GPIO_PIN_5;
}

std::string bsp::GpioPinPD5::PinName() const
{
	return "PD5";
}

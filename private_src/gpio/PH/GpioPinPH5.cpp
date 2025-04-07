#include "GpioPinPH5.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPH5::Instance)

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

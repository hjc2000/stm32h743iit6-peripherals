#include "GpioPinPE5.h"
#include "base/define.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPE5::Port()
{
	return GPIOE;
}

uint32_t bsp::GpioPinPE5::Pin()
{
	return GPIO_PIN_5;
}

std::string bsp::GpioPinPE5::PinName() const
{
	return "PE5";
}

PREINIT(bsp::GpioPinPE5::Instance)

bsp::GpioPinPE5 &bsp::GpioPinPE5::Instance()
{
	static GpioPinPE5 o{};
	return o;
}

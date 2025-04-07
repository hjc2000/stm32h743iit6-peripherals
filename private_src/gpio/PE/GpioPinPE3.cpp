#include "GpioPinPE3.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPE3::Instance)

bsp::GpioPinPE3 &bsp::GpioPinPE3::Instance()
{
	static GpioPinPE3 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPE3::Port()
{
	return GPIOE;
}

uint32_t bsp::GpioPinPE3::Pin()
{
	return GPIO_PIN_3;
}

std::string bsp::GpioPinPE3::PinName() const
{
	return "PE3";
}

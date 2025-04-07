#include "GpioPinPE2.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPE2::Instance)

bsp::GpioPinPE2 &bsp::GpioPinPE2::Instance()
{
	static GpioPinPE2 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPE2::Port()
{
	return GPIOE;
}

uint32_t bsp::GpioPinPE2::Pin()
{
	return GPIO_PIN_2;
}

std::string bsp::GpioPinPE2::PinName() const
{
	return "PE2";
}

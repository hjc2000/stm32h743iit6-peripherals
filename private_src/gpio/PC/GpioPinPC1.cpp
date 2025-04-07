#include "GpioPinPC1.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPC1::Instance)

bsp::GpioPinPC1 &bsp::GpioPinPC1::Instance()
{
	static GpioPinPC1 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPC1::Port()
{
	return GPIOC;
}

uint32_t bsp::GpioPinPC1::Pin()
{
	return GPIO_PIN_1;
}

std::string bsp::GpioPinPC1::PinName() const
{
	return "PC1";
}

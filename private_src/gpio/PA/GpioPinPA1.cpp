#include "GpioPinPA1.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPA1::Instance);

bsp::GpioPinPA1 &bsp::GpioPinPA1::Instance()
{
	static GpioPinPA1 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPA1::Port()
{
	return GPIOA;
}

uint32_t bsp::GpioPinPA1::Pin()
{
	return GPIO_PIN_1;
}

std::string bsp::GpioPinPA1::PinName() const
{
	return "PA1";
}

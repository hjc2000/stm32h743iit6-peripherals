#include "GpioPinPE4.h"
#include "base/define.h"
#include <hal.h>

GPIO_TypeDef *bsp::GpioPinPE4::Port()
{
	return GPIOE;
}

uint32_t bsp::GpioPinPE4::Pin()
{
	return GPIO_PIN_4;
}

PREINIT(bsp::GpioPinPE4::Instance)

bsp::GpioPinPE4 &bsp::GpioPinPE4::Instance()
{
	static GpioPinPE4 o{};
	return o;
}

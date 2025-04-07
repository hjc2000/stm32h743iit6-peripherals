#include "GpioPinPB12.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPB12::Instance);

bsp::GpioPinPB12 &bsp::GpioPinPB12::Instance()
{
	static GpioPinPB12 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPB12::Port()
{
	return GPIOB;
}

uint32_t bsp::GpioPinPB12::Pin()
{
	return GPIO_PIN_12;
}

std::string bsp::GpioPinPB12::PinName() const
{
	return "PB12";
}

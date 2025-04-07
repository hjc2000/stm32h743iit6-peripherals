#include "GpioPinPB5.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPB5::Instance);

bsp::GpioPinPB5 &bsp::GpioPinPB5::Instance()
{
	static GpioPinPB5 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPB5::Port()
{
	return GPIOB;
}

uint32_t bsp::GpioPinPB5::Pin()
{
	return GPIO_PIN_5;
}

std::string bsp::GpioPinPB5::PinName() const
{
	return "PB5";
}

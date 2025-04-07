#include "GpioPinPB0.h"
#include "base/define.h"

PREINIT(bsp::GpioPinPB0::Instance)

bsp::GpioPinPB0 &bsp::GpioPinPB0::Instance()
{
	static GpioPinPB0 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPB0::Port()
{
	return GPIOB;
}

uint32_t bsp::GpioPinPB0::Pin()
{
	return GPIO_PIN_0;
}

std::string bsp::GpioPinPB0::PinName() const
{
	return "PB0";
}

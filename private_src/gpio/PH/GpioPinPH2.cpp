#include "GpioPinPH2.h"
#include "base/define.h"
#include <hal.h>

PREINIT(bsp::GpioPinPH2::Instance)

bsp::GpioPinPH2 &bsp::GpioPinPH2::Instance()
{
	static GpioPinPH2 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPH2::Port()
{
	return GPIOH;
}

uint32_t bsp::GpioPinPH2::Pin()
{
	return GPIO_PIN_2;
}

std::string bsp::GpioPinPH2::PinName() const
{
	return "PH2";
}

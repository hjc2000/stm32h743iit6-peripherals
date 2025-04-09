#include "GpioPinPA10.h"
#include "base/define.h"

PREINIT(bsp::GpioPinPA10::Instance);

bsp::GpioPinPA10 &bsp::GpioPinPA10::Instance()
{
	static GpioPinPA10 o{};
	return o;
}

GPIO_TypeDef *bsp::GpioPinPA10::Port()
{
	return GPIOA;
}

uint32_t bsp::GpioPinPA10::Pin()
{
	return GPIO_PIN_10;
}

std::string bsp::GpioPinPA10::PinName() const
{
	return "PA10";
}

void bsp::GpioPinPA10::OpenAsAlternateFunctionMode(std::string function_name,
												   bsp::IGpioPinPullMode pull_mode,
												   bsp::IGpioPinDriver driver_mode)
{
	if (_is_open)
	{
		throw std::runtime_error{PinName() + " 已经打开"};
	}

	bsp::EnableClock(Port());
	GPIO_InitTypeDef def{};

	if (function_name == "usart1")
	{
		def.Alternate = GPIO_AF7_USART1;
	}
	else
	{
		throw std::invalid_argument{"不支持的 AlternateFunction"};
	}

	switch (pull_mode)
	{
	default:
	case bsp::IGpioPinPullMode::NoPull:
		{
			def.Pull = GPIO_NOPULL;
			break;
		}
	case bsp::IGpioPinPullMode::PullUp:
		{
			def.Pull = GPIO_PULLUP;
			break;
		}
	case bsp::IGpioPinPullMode::PullDown:
		{
			def.Pull = GPIO_PULLDOWN;
			break;
		}
	}

	switch (driver_mode)
	{
	case bsp::IGpioPinDriver::PushPull:
		{
			def.Mode = GPIO_MODE_AF_PP;
			break;
		}
	case bsp::IGpioPinDriver::OpenDrain:
		{
			def.Mode = GPIO_MODE_AF_OD;
			break;
		}
	default:
		{
			throw std::invalid_argument{"不支持的 Driver"};
		}
	}

	def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	def.Pin = Pin();
	HAL_GPIO_Init(Port(), &def);
	_is_open = true;
}

#include "gpio.h" // IWYU pragma: keep

void bsp::gpio::enable_clock(GPIO_TypeDef *port)
{
	if (port == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (port == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (port == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (port == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if (port == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if (port == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if (port == GPIOG)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	else if (port == GPIOH)
	{
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
}

GPIO_TypeDef *bsp::gpio::to_defined_value(base::gpio::PortEnum value)
{
	switch (value)
	{
	case base::gpio::PortEnum::PortA:
		{
			return GPIOA;
		}
	case base::gpio::PortEnum::PortB:
		{
			return GPIOB;
		}
	case base::gpio::PortEnum::PortC:
		{
			return GPIOC;
		}
	case base::gpio::PortEnum::PortD:
		{
			return GPIOD;
		}
	case base::gpio::PortEnum::PortE:
		{
			return GPIOE;
		}
	case base::gpio::PortEnum::PortF:
		{
			return GPIOF;
		}
	case base::gpio::PortEnum::PortG:
		{
			return GPIOG;
		}
	case base::gpio::PortEnum::PortH:
		{
			return GPIOH;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法端口。"};
		}
	}
}

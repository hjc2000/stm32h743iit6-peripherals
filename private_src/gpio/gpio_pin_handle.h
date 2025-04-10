#pragma once
#include "base/peripheral/gpio_handle.h" // IWYU pragma: export
#include "hal.h"
#include <cstdint>

class base::gpio::gpio_pin_handle
{
public:
	gpio_pin_handle(base::gpio::PortEnum port,
					uint32_t pin);

	~gpio_pin_handle();

	base::gpio::PortEnum _port_enum{};
	GPIO_TypeDef *_port = nullptr;
	uint32_t _pin = 0;
};

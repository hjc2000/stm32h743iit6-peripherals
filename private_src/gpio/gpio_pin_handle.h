#pragma once
#include "base/peripheral/gpio_handle.h" // IWYU pragma: export
#include "hal.h"
#include <cstdint>

class base::gpio::gpio_pin_handle
{
private:
	base::gpio::PortEnum _port_enum{};
	GPIO_TypeDef *_port = nullptr;
	uint32_t _pin = 0;

	void enable_clock();

public:
	gpio_pin_handle(base::gpio::PortEnum port, uint32_t pin);
	~gpio_pin_handle();

	void initialize_as_input_mode(base::gpio::PullMode pull_mode,
								  base::gpio::TriggerEdge trigger_edge);

	void initialize_as_output_mode(base::gpio::PullMode pull_mode,
								   base::gpio::DriveMode drive_mode);

	void initialize_as_alternate_function_mode(base::gpio::AlternateFunction af,
											   base::gpio::PullMode pull_mode,
											   base::gpio::DriveMode drive_mode);
};

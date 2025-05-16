#pragma once
#include "base/embedded/gpio/gpio_parameter.h"
#include "hal.h"

namespace bsp
{
	namespace gpio
	{
		void enable_clock(GPIO_TypeDef *port);

		///
		/// @brief 将端口枚举转为 HAL 库的端口实例指针。
		///
		/// @param value
		/// @return
		///
		GPIO_TypeDef *to_defined_value(base::gpio::PortEnum value);

		///
		/// @brief 将引脚号转为 HAL 库的引脚宏定义。
		///
		/// @param pin
		/// @return
		///
		uint32_t to_pin_define_value(uint32_t pin);

		uint32_t to_defined_value(base::gpio::AlternateFunction af);

	} // namespace gpio
} // namespace bsp

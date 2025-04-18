#include "gpio_pin_handle.h"

base::gpio::sp_gpio_pin_handle base::gpio::open(base::gpio::PortEnum port, uint32_t pin)
{
	base::gpio::sp_gpio_pin_handle handle{new base::gpio::gpio_pin_handle{port, pin}};
	return handle;
}

/* #region 全局初始化函数 */

void base::gpio::initialize_as_input_mode(base::gpio::gpio_pin_handle &h,
										  base::gpio::PullMode pull_mode,
										  base::gpio::TriggerEdge trigger_edge)
{
	h.initialize_as_input_mode(pull_mode, trigger_edge);
}

void base::gpio::initialize_as_output_mode(base::gpio::gpio_pin_handle &h,
										   base::gpio::PullMode pull_mode,
										   base::gpio::DriveMode drive_mode)
{
	h.initialize_as_output_mode(pull_mode, drive_mode);
}

void base::gpio::initialize_as_alternate_function_mode(base::gpio::gpio_pin_handle &h,
													   base::gpio::AlternateFunction af,
													   base::gpio::PullMode pull_mode,
													   base::gpio::DriveMode drive_mode)
{
	h.initialize_as_alternate_function_mode(af, pull_mode, drive_mode);
}

/* #endregion */

std::string base::gpio::pin_name(base::gpio::gpio_pin_handle &h);

/* #region 全局读写引脚函数 */

bool base::gpio::read_pin(base::gpio::gpio_pin_handle &h)
{
	return h.read_pin();
}

void base::gpio::write_pin(base::gpio::gpio_pin_handle &h, bool value)
{
	h.write_pin(value);
}

void base::gpio::toggle_pin(base::gpio::gpio_pin_handle &h)
{
	h.toggle_pin();
}

/* #endregion */

/* #region 中断回调 */

void base::gpio::register_interrupt_callback(base::gpio::gpio_pin_handle &h,
											 std::function<void()> const &callback_func)
{
	h.register_interrupt_callback(callback_func);
}

void base::gpio::unregister_interrupt_callback(base::gpio::gpio_pin_handle &h)
{
	h.unregister_interrupt_callback();
}

/* #endregion */

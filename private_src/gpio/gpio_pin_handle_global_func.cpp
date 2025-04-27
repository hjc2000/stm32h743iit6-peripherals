#include "gpio_pin_handle.h"

std::shared_ptr<base::gpio::gpio_pin_handle> base::gpio::open(base::gpio::PortEnum port, uint32_t pin)
{
	std::shared_ptr<base::gpio::gpio_pin_handle> handle{new base::gpio::gpio_pin_handle{port, pin}};
	return handle;
}

/* #region 全局初始化函数 */

void base::gpio::initialize_as_input_mode(base::gpio::gpio_pin_handle &h,
										  base::gpio::PullMode pull_mode,
										  base::gpio::TriggerEdge trigger_edge)
{
	h.InitializeAsInputMode(pull_mode, trigger_edge);
}

void base::gpio::initialize_as_output_mode(base::gpio::gpio_pin_handle &h,
										   base::gpio::PullMode pull_mode,
										   base::gpio::DriveMode drive_mode)
{
	h.InitializeAsOutputMode(pull_mode, drive_mode);
}

void base::gpio::initialize_as_output_mode(base::gpio::gpio_pin_handle &h)
{
	h.InitializeAsOutputMode();
}

void base::gpio::initialize_as_alternate_function_mode(base::gpio::gpio_pin_handle &h,
													   base::gpio::AlternateFunction af,
													   base::gpio::PullMode pull_mode,
													   base::gpio::DriveMode drive_mode)
{
	h.InitializeAsAlternateFunctionMode(af, pull_mode, drive_mode);
}

/* #endregion */

/* #region 引脚信息 */

base::gpio::PortEnum base::gpio::port(base::gpio::gpio_pin_handle &h)
{
	return h.Port();
}

uint32_t base::gpio::pin(base::gpio::gpio_pin_handle &h)
{
	return h.Pin();
}

/* #endregion */

/* #region 全局读写引脚函数 */

bool base::gpio::read_pin(base::gpio::gpio_pin_handle &h)
{
	return h.ReadPin();
}

void base::gpio::write_pin(base::gpio::gpio_pin_handle &h, bool value)
{
	h.WritePin(value);
}

void base::gpio::toggle_pin(base::gpio::gpio_pin_handle &h)
{
	h.TogglePin();
}

/* #endregion */

/* #region 中断回调 */

void base::gpio::register_interrupt_callback(base::gpio::gpio_pin_handle &h,
											 std::function<void()> const &callback_func)
{
	h.RegisterInterruptCallback(callback_func);
}

void base::gpio::unregister_interrupt_callback(base::gpio::gpio_pin_handle &h)
{
	h.UnregisterInterruptCallback();
}

/* #endregion */

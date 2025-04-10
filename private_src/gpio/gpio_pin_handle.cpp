#include "gpio_pin_handle.h"
#include "base/define.h"
#include "base/peripheral/gpio_parameter.h"
#include "base/string/define.h"
#include <array>
#include <bitset>
#include <cstdint>
#include <stdexcept>

namespace
{
	constexpr int PortCount()
	{
		return static_cast<int>(base::gpio::PortEnum::PortP) + 1;
	}

	/* #region 使用状态管理 */

	std::array<std::bitset<16>, PortCount()> &PortsUsageStates()
	{
		static std::array<std::bitset<16>, PortCount()> o{};
		return o;
	}

	PREINIT(PortsUsageStates)

	bool IsUsed(base::gpio::PortEnum port, uint32_t pin)
	{
		return PortsUsageStates()[static_cast<int>(port)][pin];
	}

	void SetAsUsed(base::gpio::PortEnum port, uint32_t pin)
	{
		if (static_cast<int>(port) >= PortCount())
		{
			throw std::invalid_argument{CODE_POS_STR};
		}

		PortsUsageStates()[static_cast<int>(port)][pin] = true;
	}

	void SetAsUnused(base::gpio::PortEnum port, uint32_t pin)
	{
		if (static_cast<int>(port) >= PortCount())
		{
			throw std::invalid_argument{CODE_POS_STR};
		}

		PortsUsageStates()[static_cast<int>(port)][pin] = false;
	}

	/* #endregion */

	constexpr GPIO_TypeDef *ToPort(base::gpio::PortEnum value)
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
				throw std::invalid_argument{CODE_POS_STR};
			}
		}
	}

} // namespace

base::gpio::gpio_pin_handle::gpio_pin_handle(base::gpio::PortEnum port,
											 uint32_t pin)
{
	_port_enum = port;
	_port = ToPort(port);
	_pin = pin;
	SetAsUsed(port, pin);
}

base::gpio::gpio_pin_handle::~gpio_pin_handle()
{
	SetAsUnused(_port_enum, _pin);
}

/* #region 打开函数 */

base::gpio::sp_gpio_pin_handle base::gpio::open_as_input_mode(base::gpio::PortEnum port,
															  uint32_t pin,
															  base::gpio::PullMode pull_mode,
															  base::gpio::TriggerEdge trigger_edge)
{
	if (IsUsed(port, pin))
	{
		throw std::runtime_error{CODE_POS_STR + "此设备被占用"};
	}

	base::gpio::sp_gpio_pin_handle h{new gpio_pin_handle{port, pin}};
	return h;
}

base::gpio::sp_gpio_pin_handle base::gpio::open_as_output_mode(base::gpio::PortEnum port,
															   uint32_t pin,
															   base::gpio::PullMode pull_mode,
															   base::gpio::DriveMode drive_mode);

base::gpio::sp_gpio_pin_handle base::gpio::open_as_alternate_function_mode(base::gpio::PortEnum port,
																		   uint32_t pin,
																		   base::gpio::AlternateFunction af,
																		   base::gpio::PullMode pull_mode,
																		   base::gpio::DriveMode drive_mode);

/* #endregion */

std::string base::gpio::pin_name(base::gpio::sp_gpio_pin_handle const &h);

bool base::gpio::read_pin(base::gpio::sp_gpio_pin_handle const &h);
void base::gpio::write_pin(base::gpio::sp_gpio_pin_handle const &h, bool value);
void base::gpio::toggle_pin(base::gpio::sp_gpio_pin_handle const &h);

///
/// @brief 注册中断回调函数。
///
/// @param h
/// @param callback_func
///
void base::gpio::register_interrupt_callback(base::gpio::sp_gpio_pin_handle const &h, std::function<void()> callback_func);

///
/// @brief 取消注册中断回调函数。
///
/// @param h
///
void base::gpio::unregister_interrupt_callback(base::gpio::sp_gpio_pin_handle const &h);

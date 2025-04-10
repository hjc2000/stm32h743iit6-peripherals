#include "gpio_pin_handle.h"
#include "base/define.h"
#include "base/LockGuard.h"
#include "base/peripheral/gpio_parameter.h"
#include "base/string/define.h"
#include "base/task/IMutex.h"
#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <stdexcept>

namespace
{
	constexpr int PortCount()
	{
		return static_cast<int>(base::gpio::PortEnum::PortP) + 1;
	}

	/* #region UsageStateManager */

	class UsageStateManager
	{
	private:
		UsageStateManager() = default;

		std::array<std::bitset<16>, PortCount()> _states{};
		std::shared_ptr<base::IMutex> _lock = base::CreateIMutex();

	public:
		static UsageStateManager &Instance()
		{
			static UsageStateManager o;
			return o;
		}

		bool IsUsed(base::gpio::PortEnum port, uint32_t pin)
		{
			if (static_cast<int>(port) >= PortCount())
			{
				throw std::invalid_argument{CODE_POS_STR};
			}

			base::LockGuard g{*_lock};
			return _states[static_cast<int>(port)][pin];
		}

		void SetAsUsed(base::gpio::PortEnum port, uint32_t pin)
		{
			if (static_cast<int>(port) >= PortCount())
			{
				throw std::invalid_argument{CODE_POS_STR};
			}

			base::LockGuard g{*_lock};
			_states[static_cast<int>(port)][pin] = true;
		}

		void SetAsUnused(base::gpio::PortEnum port, uint32_t pin)
		{
			if (static_cast<int>(port) >= PortCount())
			{
				throw std::invalid_argument{CODE_POS_STR};
			}

			base::LockGuard g{*_lock};
			_states[static_cast<int>(port)][pin] = false;
		}
	};

	PREINIT(UsageStateManager::Instance)

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
	UsageStateManager::Instance().SetAsUsed(port, pin);
}

base::gpio::gpio_pin_handle::~gpio_pin_handle()
{
	UsageStateManager::Instance().SetAsUnused(_port_enum, _pin);
}

/* #region 打开函数 */

base::gpio::sp_gpio_pin_handle base::gpio::open_as_input_mode(base::gpio::PortEnum port,
															  uint32_t pin,
															  base::gpio::PullMode pull_mode,
															  base::gpio::TriggerEdge trigger_edge)
{
	if (UsageStateManager::Instance().IsUsed(port, pin))
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

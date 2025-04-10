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

		void SetAsUsed(base::gpio::PortEnum port, uint32_t pin)
		{
			if (static_cast<int>(port) >= PortCount())
			{
				throw std::invalid_argument{CODE_POS_STR};
			}

			base::LockGuard g{*_lock};
			if (_states[static_cast<int>(port)][pin])
			{
				throw std::runtime_error{CODE_POS_STR + "已经占用了，无法再次占用。"};
			}

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

void base::gpio::gpio_pin_handle::enable_clock()
{
	if (_port == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if (_port == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if (_port == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (_port == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if (_port == GPIOE)
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();
	}
	else if (_port == GPIOF)
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else if (_port == GPIOG)
	{
		__HAL_RCC_GPIOG_CLK_ENABLE();
	}
	else if (_port == GPIOH)
	{
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
}

base::gpio::gpio_pin_handle::gpio_pin_handle(base::gpio::PortEnum port, uint32_t pin)
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

/* #region 初始化函数 */

void base::gpio::gpio_pin_handle::initialize_as_input_mode(base::gpio::PullMode pull_mode,
														   base::gpio::TriggerEdge trigger_edge)
{
	enable_clock();
	GPIO_InitTypeDef def{};
	switch (pull_mode)
	{
	default:
	case base::gpio::PullMode::NoPull:
		{
			def.Pull = GPIO_NOPULL;
			break;
		}
	case base::gpio::PullMode::PullUp:
		{
			def.Pull = GPIO_PULLUP;
			break;
		}
	case base::gpio::PullMode::PullDown:
		{
			def.Pull = GPIO_PULLDOWN;
			break;
		}
	}

	switch (trigger_edge)
	{
	default:
	case base::gpio::TriggerEdge::Disable:
		{
			def.Mode = GPIO_MODE_INPUT;
			break;
		}
	case base::gpio::TriggerEdge::RisingEdge:
		{
			def.Mode = GPIO_MODE_IT_RISING;
			break;
		}
	case base::gpio::TriggerEdge::FallingEdge:
		{
			def.Mode = GPIO_MODE_IT_FALLING;
			break;
		}
	case base::gpio::TriggerEdge::BothEdge:
		{
			def.Mode = GPIO_MODE_IT_RISING_FALLING;
			break;
		}
	}

	def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	def.Pin = _pin;
	HAL_GPIO_Init(_port, &def);
}

void base::gpio::gpio_pin_handle::initialize_as_output_mode(base::gpio::PullMode pull_mode,
															base::gpio::DriveMode drive_mode)
{
	enable_clock();
	GPIO_InitTypeDef def{};
	switch (pull_mode)
	{
	default:
	case base::gpio::PullMode::NoPull:
		{
			def.Pull = GPIO_NOPULL;
			break;
		}
	case base::gpio::PullMode::PullUp:
		{
			def.Pull = GPIO_PULLUP;
			break;
		}
	case base::gpio::PullMode::PullDown:
		{
			def.Pull = GPIO_PULLDOWN;
			break;
		}
	}

	switch (drive_mode)
	{
	case base::gpio::DriveMode::PushPull:
		{
			def.Mode = GPIO_MODE_OUTPUT_PP;
			break;
		}
	case base::gpio::DriveMode::OpenDrain:
		{
			def.Mode = GPIO_MODE_OUTPUT_OD;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "不支持的驱动模式。"};
		}
	}

	def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	def.Pin = _pin;
	HAL_GPIO_Init(_port, &def);
}

void base::gpio::gpio_pin_handle::initialize_as_alternate_function_mode(base::gpio::AlternateFunction af,
																		base::gpio::PullMode pull_mode,
																		base::gpio::DriveMode drive_mode)
{
	enable_clock();
}

/* #endregion */

/* #region 全局初始化函数 */

void base::gpio::initialize_as_input_mode(base::gpio::sp_gpio_pin_handle const &h,
										  base::gpio::PullMode pull_mode,
										  base::gpio::TriggerEdge trigger_edge)
{
	h->initialize_as_input_mode(pull_mode, trigger_edge);
}

void base::gpio::initialize_as_output_mode(base::gpio::sp_gpio_pin_handle const &h,
										   base::gpio::PullMode pull_mode,
										   base::gpio::DriveMode drive_mode)
{
	h->initialize_as_output_mode(pull_mode, drive_mode);
}

void base::gpio::initialize_as_alternate_function_mode(base::gpio::sp_gpio_pin_handle const &h,
													   base::gpio::AlternateFunction af,
													   base::gpio::PullMode pull_mode,
													   base::gpio::DriveMode drive_mode)
{
	h->initialize_as_alternate_function_mode(af, pull_mode, drive_mode);
}

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

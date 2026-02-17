#include "base/embedded/interrupt/exti.h"
#include "base/embedded/interrupt/interrupt.h"
#include "base/string/define.h"
#include "hal.h"
#include <cstdint>
#include <stdexcept>

namespace
{
	///
	/// @brief 默认的中断优先级。
	///
	///
	int32_t _default_priority = 5;

	std::function<void()> _exti_interrupt_callback_handlers[16]{};

} // namespace

int32_t base::exti::default_priority()
{
	return _default_priority;
}

void base::exti::set_default_priority(int32_t priority)
{
	_default_priority = priority;
}

/* #region 注册回调 */

void base::exti::register_callback(int32_t line_id,
								   int32_t priority,
								   std::function<void()> const &callback)
{
	switch (line_id)
	{
	case 0:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn));
			_exti_interrupt_callback_handlers[0] = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn), priority);
			break;
		}
	case 1:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn));
			_exti_interrupt_callback_handlers[1] = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn), priority);
			break;
		}
	case 2:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn));
			_exti_interrupt_callback_handlers[2] = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn), priority);
			break;
		}
	case 3:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn));
			_exti_interrupt_callback_handlers[3] = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn), priority);
			break;
		}
	case 4:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn));
			_exti_interrupt_callback_handlers[4] = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn), priority);
			break;
		}
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI9_5_IRQn));
			_exti_interrupt_callback_handlers[line_id] = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI9_5_IRQn), priority);
			break;
		}
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI15_10_IRQn));
			_exti_interrupt_callback_handlers[line_id] = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI15_10_IRQn), priority);
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "pin 超出范围。"};
		}
	}
}

void base::exti::register_callback(int32_t line_id, std::function<void()> const &callback)
{
	base::exti::register_callback(line_id, _default_priority, callback);
}

/* #endregion */

void base::exti::unregister_callback(int32_t line_id)
{
	switch (line_id)
	{
	case 0:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn));
			_exti_interrupt_callback_handlers[0] = nullptr;
			break;
		}
	case 1:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn));
			_exti_interrupt_callback_handlers[1] = nullptr;
			break;
		}
	case 2:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn));
			_exti_interrupt_callback_handlers[2] = nullptr;
			break;
		}
	case 3:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn));
			_exti_interrupt_callback_handlers[3] = nullptr;
			break;
		}
	case 4:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn));
			_exti_interrupt_callback_handlers[4] = nullptr;
			break;
		}
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI9_5_IRQn));
			_exti_interrupt_callback_handlers[line_id] = nullptr;
			break;
		}
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI15_10_IRQn));
			_exti_interrupt_callback_handlers[line_id] = nullptr;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "pin 超出范围。"};
		}
	}
}

extern "C"
{
	/* #region 中断服务函数 */

	void EXTI0_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
		}
		catch (...)
		{
		}
	}

	void EXTI1_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
		}
		catch (...)
		{
		}
	}

	void EXTI2_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
		}
		catch (...)
		{
		}
	}

	void EXTI3_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
		}
		catch (...)
		{
		}
	}

	void EXTI4_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
		}
		catch (...)
		{
		}
	}

	void EXTI9_5_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
		}
		catch (...)
		{
		}
	}

	void EXTI15_10_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
		}
		catch (...)
		{
		}
	}

	/* #endregion */

	///
	/// @brief 重写 HAL 库中的 weak 版本
	///
	/// @param pin
	///
	void HAL_GPIO_EXTI_Callback(uint16_t pin)
	{
		switch (pin)
		{
		case GPIO_PIN_0:
			{
				if (_exti_interrupt_callback_handlers[0])
				{
					_exti_interrupt_callback_handlers[0]();
				}

				break;
			}
		case GPIO_PIN_1:
			{
				if (_exti_interrupt_callback_handlers[1])
				{
					_exti_interrupt_callback_handlers[1]();
				}

				break;
			}
		case GPIO_PIN_2:
			{
				if (_exti_interrupt_callback_handlers[2])
				{
					_exti_interrupt_callback_handlers[2]();
				}

				break;
			}
		case GPIO_PIN_3:
			{
				if (_exti_interrupt_callback_handlers[3])
				{
					_exti_interrupt_callback_handlers[3]();
				}

				break;
			}
		case GPIO_PIN_4:
			{
				if (_exti_interrupt_callback_handlers[4])
				{
					_exti_interrupt_callback_handlers[4]();
				}

				break;
			}
		case GPIO_PIN_5:
			{
				if (_exti_interrupt_callback_handlers[5])
				{
					_exti_interrupt_callback_handlers[5]();
				}

				break;
			}
		case GPIO_PIN_6:
			{
				if (_exti_interrupt_callback_handlers[6])
				{
					_exti_interrupt_callback_handlers[6]();
				}

				break;
			}
		case GPIO_PIN_7:
			{
				if (_exti_interrupt_callback_handlers[7])
				{
					_exti_interrupt_callback_handlers[7]();
				}

				break;
			}
		case GPIO_PIN_8:
			{
				if (_exti_interrupt_callback_handlers[8])
				{
					_exti_interrupt_callback_handlers[8]();
				}

				break;
			}
		case GPIO_PIN_9:
			{
				if (_exti_interrupt_callback_handlers[9])
				{
					_exti_interrupt_callback_handlers[9]();
				}

				break;
			}
		case GPIO_PIN_10:
			{
				if (_exti_interrupt_callback_handlers[10])
				{
					_exti_interrupt_callback_handlers[10]();
				}

				break;
			}
		case GPIO_PIN_11:
			{
				if (_exti_interrupt_callback_handlers[11])
				{
					_exti_interrupt_callback_handlers[11]();
				}

				break;
			}
		case GPIO_PIN_12:
			{
				if (_exti_interrupt_callback_handlers[12])
				{
					_exti_interrupt_callback_handlers[12]();
				}

				break;
			}
		case GPIO_PIN_13:
			{
				if (_exti_interrupt_callback_handlers[13])
				{
					_exti_interrupt_callback_handlers[13]();
				}

				break;
			}
		case GPIO_PIN_14:
			{
				if (_exti_interrupt_callback_handlers[14])
				{
					_exti_interrupt_callback_handlers[14]();
				}

				break;
			}
		case GPIO_PIN_15:
			{
				if (_exti_interrupt_callback_handlers[15])
				{
					_exti_interrupt_callback_handlers[15]();
				}

				break;
			}
		default:
			{
				break;
			}
		}
	}
} // extern "C"

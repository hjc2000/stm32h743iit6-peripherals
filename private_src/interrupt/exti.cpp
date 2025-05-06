#include "base/embedded/interrupt/exti.h"
#include "base/embedded/interrupt/interrupt.h"
#include "hal.h"

namespace
{
	std::function<void()> _on_exti0_interrupt;
	std::function<void()> _on_exti1_interrupt;
	std::function<void()> _on_exti2_interrupt;
	std::function<void()> _on_exti3_interrupt;
	std::function<void()> _on_exti4_interrupt;
	std::function<void()> _on_exti13_interrupt;

} // namespace

void base::exti::register_callback(int line_id, std::function<void()> const &callback)
{
	switch (line_id)
	{
	case 0:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn));
			_on_exti0_interrupt = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn), 4);
			break;
		}
	case 1:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn));
			_on_exti1_interrupt = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn), 4);
			break;
		}
	case 2:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn));
			_on_exti2_interrupt = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn), 4);
			break;
		}
	case 3:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn));
			_on_exti3_interrupt = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn), 4);
			break;
		}
	case 4:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn));
			_on_exti4_interrupt = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn), 4);
			break;
		}
	case 13:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI15_10_IRQn));
			_on_exti13_interrupt = callback;
			base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI15_10_IRQn), 4);
			break;
		}
	default:
		{
			throw std::invalid_argument{"pin 超出范围。"};
		}
	}
}

void base::exti::unregister_callback(int line_id)
{
	switch (line_id)
	{
	case 0:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI0_IRQn));
			_on_exti0_interrupt = nullptr;
			break;
		}
	case 1:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI1_IRQn));
			_on_exti1_interrupt = nullptr;
			break;
		}
	case 2:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI2_IRQn));
			_on_exti2_interrupt = nullptr;
			break;
		}
	case 3:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI3_IRQn));
			_on_exti3_interrupt = nullptr;
			break;
		}
	case 4:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI4_IRQn));
			_on_exti4_interrupt = nullptr;
			break;
		}
	case 13:
		{
			base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::EXTI15_10_IRQn));
			_on_exti13_interrupt = nullptr;
			break;
		}
	default:
		{
			throw std::invalid_argument{"pin 超出范围。"};
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

	void EXTI15_10_IRQHandler()
	{
		try
		{
			HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
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
				if (_on_exti0_interrupt)
				{
					_on_exti0_interrupt();
				}

				break;
			}
		case GPIO_PIN_1:
			{
				if (_on_exti1_interrupt)
				{
					_on_exti1_interrupt();
				}

				break;
			}
		case GPIO_PIN_2:
			{
				if (_on_exti2_interrupt)
				{
					_on_exti2_interrupt();
				}

				break;
			}
		case GPIO_PIN_3:
			{
				if (_on_exti3_interrupt)
				{
					_on_exti3_interrupt();
				}

				break;
			}
		case GPIO_PIN_4:
			{
				if (_on_exti4_interrupt)
				{
					_on_exti4_interrupt();
				}

				break;
			}
		case GPIO_PIN_13:
			{
				if (_on_exti13_interrupt)
				{
					_on_exti13_interrupt();
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

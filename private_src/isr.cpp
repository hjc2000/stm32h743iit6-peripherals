#include "bsp-interface/di/interrupt.h"
#include "hal.h"

extern "C"
{
	void WWDG1_IRQHandler()
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::WWDG_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}

	///
	/// @brief 以太网中断服务函数。
	/// @param
	///
	void ETH_IRQHandler(void)
	{
		std::function<void()> &func = bsp::di::interrupt::IsrManager().GetIsr(static_cast<uint32_t>(IRQn_Type::ETH_IRQn));
		try
		{
			func();
		}
		catch (...)
		{
		}
	}
}

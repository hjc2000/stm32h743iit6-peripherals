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
}

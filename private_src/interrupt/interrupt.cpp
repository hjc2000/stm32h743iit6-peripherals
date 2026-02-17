#include "bsp-interface/di/interrupt.h"
#include "base/embedded/interrupt/interrupt.h"
#include "hal.h"

void base::interrupt::DisableGlobalInterruptionImpelement::disable_global_interrupt() noexcept
{
	__disable_irq();
}

void base::interrupt::DisableGlobalInterruptionImpelement::enable_global_interrupt() noexcept
{
	__enable_irq();
}

void base::interrupt::disable_interrupt(int32_t irq) noexcept
{
	HAL_NVIC_DisableIRQ(static_cast<IRQn_Type>(irq));
}

void base::interrupt::enable_interrupt(int32_t irq) noexcept
{
	// 硬件中断控制器中每一个中断都天生自带一个默认的优先级，并且这种优先级一般是由中断号的大小
	// 自然而然地就决定了。所以这里只要使能中断就行了，使用硬件中断控制器自带的优先级。
	HAL_NVIC_EnableIRQ(static_cast<IRQn_Type>(irq));
}

void base::interrupt::enable_interrupt(int32_t irq, int32_t priority) noexcept
{
	// 中断优先级可能需要先禁用中断后才能改。不知道，反正做了没错。
	base::interrupt::disable_interrupt(irq);
	HAL_NVIC_SetPriority(static_cast<IRQn_Type>(irq), priority, 0);
	HAL_NVIC_EnableIRQ(static_cast<IRQn_Type>(irq));
}

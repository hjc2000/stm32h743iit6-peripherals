#include "base/embedded/cache/cache.h"
#include "bsp-interface/di/reset_initialize.h"
#include "hal.h"

/// @brief 初始化 CPU.
/// @note 本函数需要被 DI_Initialize 调用。别的地方都不要调用。
/// @note 这里用来执行一些非常底层的 CPU 的初始化，甚至比时钟配置还要底层。
/// 例如使能 FPU，使能 cache，开启第二个核心，......等。
void DI_InitializeCPU()
{
	HAL_Init();

	MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY)
	{
	}

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	HAL_EnableCompensationCell();
	base::cache::enable();
}

#include "base/embedded/core.h"
#include "base/embedded/cache/cache.h"
#include "hal.h"

void base::core::initialize()
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

void base::core::reset()
{
	HAL_NVIC_SystemReset();
}

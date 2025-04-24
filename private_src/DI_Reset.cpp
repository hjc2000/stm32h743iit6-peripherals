#include <bsp-interface/di/reset_initialize.h>
#include <hal.h>

void DI_Reset()
{
    HAL_NVIC_SystemReset();
}

#include "base/embedded/core.h"
#include "hal.h"

void base::core::reset()
{
	HAL_NVIC_SystemReset();
}

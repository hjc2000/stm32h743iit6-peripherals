#pragma once
#include "base/peripheral/IDma.h" // IWYU pragma: export
#include "hal.h"

class base::dma::DmaContext
{
public:
	DMA_HandleTypeDef _handle{};
};

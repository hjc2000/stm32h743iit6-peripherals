#pragma once
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "memory_dma_handle.h"

namespace bsp
{
	class MemoryDma1 final :
		public base::memory_dma::memory_dma_handle
	{
	private:
		base::UsageStateManager<MemoryDma1> _usage_state_manager{};
		DMA_HandleTypeDef _handle{};

	public:
		virtual void Initialize() override
		{
			__HAL_RCC_DMA1_CLK_ENABLE();

			_handle.Instance = DMA1_Stream2;
			_handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
			_handle.Init.Request = DMA_REQUEST_MEM2MEM;
			_handle.Init.Mode = DMA_NORMAL;
			_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
			_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
			_handle.Init.MemBurst = DMA_MBURST_SINGLE;
			_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
			_handle.Init.PeriphInc = DMA_PINC_ENABLE;
			_handle.Init.MemInc = DMA_MINC_ENABLE;
			_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
			_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
			HAL_DMA_Init(&_handle);
		}

		virtual void Copy(uint8_t const *begin,
						  uint8_t const *end,
						  uint8_t *dst) override
		{
		}
	};

} // namespace bsp

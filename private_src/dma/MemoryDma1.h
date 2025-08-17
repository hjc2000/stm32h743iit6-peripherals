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

		void InitializeInterrupt();

	public:
		~MemoryDma1();

		virtual void Initialize() override;

		virtual void Copy(uint8_t const *begin,
						  uint8_t const *end,
						  uint8_t *dst) override
		{
		}
	};

} // namespace bsp

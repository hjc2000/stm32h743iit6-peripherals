#include "MemoryDma1.h" // IWYU pragma: keep
#include "base/embedded/interrupt/interrupt.h"
#include <functional>

namespace
{
	std::function<void()> _dma1_stream2_isr;

}

extern "C"
{
	void DMA1_Stream2_IRQHandler()
	{
		try
		{
			_dma1_stream2_isr();
		}
		catch (...)
		{
		}
	}
}

void bsp::MemoryDma1::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream2_IRQn));

	_dma1_stream2_isr = [this]()
	{
		HAL_DMA_IRQHandler(&_handle);
	};

	base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream2_IRQn), 10);
}

void bsp::MemoryDma1::Initialize()
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

	InitializeInterrupt();
}

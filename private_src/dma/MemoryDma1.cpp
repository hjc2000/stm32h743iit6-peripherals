#include "MemoryDma1.h" // IWYU pragma: keep
#include "base/embedded/interrupt/interrupt.h"
#include "stm32h7xx_hal_dma.h"
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

void bsp::MemoryDma1::InitializeCallback()
{
	_handle_context._handle.XferCpltCallback = [](DMA_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->OnCompleteCallback();
	};

	_handle_context._handle.XferErrorCallback = [](DMA_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->OnCompleteCallback();
	};

	_handle_context._handle.XferAbortCallback = [](DMA_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->OnCompleteCallback();
	};
}

void bsp::MemoryDma1::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream2_IRQn));

	_dma1_stream2_isr = [this]()
	{
		HAL_DMA_IRQHandler(&_handle_context._handle);
	};

	base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream2_IRQn), 10);
}

bsp::MemoryDma1::~MemoryDma1()
{
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream2_IRQn));
	_dma1_stream2_isr = nullptr;
}

void bsp::MemoryDma1::Initialize()
{
	__HAL_RCC_DMA1_CLK_ENABLE();

	_handle_context._handle.Instance = DMA1_Stream2;
	_handle_context._handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
	_handle_context._handle.Init.Request = DMA_REQUEST_MEM2MEM;
	_handle_context._handle.Init.Mode = DMA_NORMAL;
	_handle_context._handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	_handle_context._handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	_handle_context._handle.Init.MemBurst = DMA_MBURST_SINGLE;
	_handle_context._handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	_handle_context._handle.Init.PeriphInc = DMA_PINC_ENABLE;
	_handle_context._handle.Init.MemInc = DMA_MINC_ENABLE;
	_handle_context._handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	_handle_context._handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	_handle_context._handle.Init.Priority = DMA_PRIORITY_MEDIUM;
	HAL_DMA_Init(&_handle_context._handle);

	InitializeCallback();
	InitializeInterrupt();
}

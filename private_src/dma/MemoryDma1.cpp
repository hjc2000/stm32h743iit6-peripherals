#include "MemoryDma1.h" // IWYU pragma: keep
#include "base/embedded/cache/cache.h"
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
	Initialize(1);
}

void bsp::MemoryDma1::Initialize(size_t align)
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
	_handle_context._handle.Init.Priority = DMA_PRIORITY_MEDIUM;

	switch (align)
	{
	case 1:
		{
			_handle_context._handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			_handle_context._handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
			break;
		}
	case 2:
		{
			_handle_context._handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
			_handle_context._handle.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
			break;
		}
	case 4:
		{
			_handle_context._handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
			_handle_context._handle.Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法对齐方式。"};
		}
	}

	HAL_DMA_Init(&_handle_context._handle);

	InitializeCallback();
	InitializeInterrupt();
}

void bsp::MemoryDma1::Copy(uint8_t const *begin, uint8_t const *end, uint8_t *dst)
{
	base::task::MutexGuard g{_lock};

	base::cache::clean_d_cache(begin, end - begin);

	HAL_DMA_Start_IT(&_handle_context._handle,
					 reinterpret_cast<uint32_t>(begin),
					 reinterpret_cast<uint32_t>(dst),
					 static_cast<uint32_t>(end - begin));

	_complete_signal.Acquire();

	base::cache::invalidate_d_cache(dst, end - begin);
}

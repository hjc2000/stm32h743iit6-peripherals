#include "DmaStream.h"
#include "base/define.h"
#include <base/string/define.h>
#include <stdexcept>

namespace
{
	std::map<std::string, uint32_t> const &RequestMap()
	{
		static std::map<std::string, uint32_t> o{
			{"usart1_rx", DMA_REQUEST_USART1_RX},
			{"usart1_tx", DMA_REQUEST_USART1_TX},
		};

		return o;
	}

} // namespace

PREINIT(RequestMap)

void bsp::DmaStream::LinkDmaToUartTx(UART_HandleTypeDef &uart)
{
	uart.hdmatx = &_dma_handle;
	_dma_handle.Parent = &uart;
}

void bsp::DmaStream::LinkDmaToUartRx(UART_HandleTypeDef &uart)
{
	uart.hdmarx = &_dma_handle;
	_dma_handle.Parent = &uart;
}

void bsp::DmaStream::InitializeDmaProperty(base::dma::PeripheralIncrement peripheral_increment,
										   base::dma::MemoryIncrement memory_increment,
										   base::dma::PeripheralDataAlignment const &peripheral_data_alignment,
										   base::dma::MemoryDataAlignment const &memory_data_alignment,
										   base::dma::Priority priority,
										   std::string const &request)
{
	_dma_handle.Init.Mode = DMA_NORMAL;
	_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
	_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;

	if (peripheral_increment == base::dma::PeripheralIncrement::Increase)
	{
		_dma_handle.Init.PeriphInc = DMA_PINC_ENABLE;
	}
	else
	{
		_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
	}

	if (memory_increment == base::dma::MemoryIncrement::Increase)
	{
		_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
	}
	else
	{
		_dma_handle.Init.MemInc = DMA_MINC_DISABLE;
	}

	switch (peripheral_data_alignment.Value())
	{
	case 1:
		{
			_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
			break;
		}
	case 2:
		{
			_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
			break;
		}
	case 4:
		{
			_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法对齐方式"};
		}
	}

	switch (memory_data_alignment.Value())
	{
	case 1:
		{
			_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
			break;
		}
	case 2:
		{
			_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
			break;
		}
	case 4:
		{
			_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法对齐方式"};
		}
	}

	switch (priority)
	{
	case base::dma::Priority::Low:
		{
			_dma_handle.Init.Priority = DMA_PRIORITY_LOW;
			break;
		}
	case base::dma::Priority::Medium:
		{
			_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
			break;
		}
	case base::dma::Priority::High:
		{
			_dma_handle.Init.Priority = DMA_PRIORITY_HIGH;
			break;
		}
	case base::dma::Priority::VeryHigh:
		{
			_dma_handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
			break;
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法优先级"};
		}
	}

	// Request
	{
		auto it = RequestMap().find(request);
		if (it == RequestMap().end())
		{
			throw std::runtime_error{CODE_POS_STR + "不支持此请求"};
		}

		_dma_handle.Init.Request = it->second;
	}
}

void bsp::DmaStream::LinkDmaToParent(void *parent)
{
	switch (_dma_handle.Init.Request)
	{
	case DMA_REQUEST_USART1_TX:
		{
			LinkDmaToUartTx(*static_cast<UART_HandleTypeDef *>(parent));
			break;
		}
	case DMA_REQUEST_USART1_RX:
		{
			LinkDmaToUartRx(*static_cast<UART_HandleTypeDef *>(parent));
			break;
		}
	default:
		{
			throw std::runtime_error{CODE_POS_STR + "不支持的请求"};
		}
	}
}

#include "DmaStream.h"
#include "base/define.h"
#include "dma.h"
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
	bsp::dma::SetDmaProperty(_dma_handle,
							 peripheral_increment,
							 memory_increment,
							 peripheral_data_alignment,
							 memory_data_alignment,
							 priority);

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

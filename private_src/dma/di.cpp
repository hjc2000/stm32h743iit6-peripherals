#include "base/peripheral/IDma.h"
#include "DmaContext.h"
#include "hal.h"
#include "Serial.h"

namespace
{
	void SetDmaProperty(DMA_HandleTypeDef &handle,
						base::dma::PeripheralIncrement peripheral_increment,
						base::dma::MemoryIncrement memory_increment,
						base::dma::PeripheralDataAlignment const &peripheral_data_alignment,
						base::dma::MemoryDataAlignment const &memory_data_alignment,
						base::dma::Priority priority)
	{
		handle.Init.Mode = DMA_NORMAL;
		handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		handle.Init.MemBurst = DMA_MBURST_SINGLE;
		handle.Init.PeriphBurst = DMA_PBURST_SINGLE;

		if (peripheral_increment == base::dma::PeripheralIncrement::Increase)
		{
			handle.Init.PeriphInc = DMA_PINC_ENABLE;
		}
		else
		{
			handle.Init.PeriphInc = DMA_PINC_DISABLE;
		}

		if (memory_increment == base::dma::MemoryIncrement::Increase)
		{
			handle.Init.MemInc = DMA_MINC_ENABLE;
		}
		else
		{
			handle.Init.MemInc = DMA_MINC_DISABLE;
		}

		switch (peripheral_data_alignment.Value())
		{
		case 1:
			{
				handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
				break;
			}
		case 2:
			{
				handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
				break;
			}
		case 4:
			{
				handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
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
				handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
				break;
			}
		case 2:
			{
				handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
				break;
			}
		case 4:
			{
				handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
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
				handle.Init.Priority = DMA_PRIORITY_LOW;
				break;
			}
		case base::dma::Priority::Medium:
			{
				handle.Init.Priority = DMA_PRIORITY_MEDIUM;
				break;
			}
		case base::dma::Priority::High:
			{
				handle.Init.Priority = DMA_PRIORITY_HIGH;
				break;
			}
		case base::dma::Priority::VeryHigh:
			{
				handle.Init.Priority = DMA_PRIORITY_VERY_HIGH;
				break;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法优先级"};
			}
		}
	}

	void EnableClock(DMA_HandleTypeDef &handle)
	{
		if (handle.Instance == DMA1_Stream0 ||
			handle.Instance == DMA1_Stream1)
		{
			__HAL_RCC_DMA1_CLK_ENABLE();
		}
	}

} // namespace

/* #region 全局的 DMA 打开函数 */

void base::dma::OpenForSerialReceiving(base::dma::IDma *dma, base::serial::ISerial *serial)
{
	EnableClock(dma->Context()._handle);
	dma->Context()._handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dma->Context()._handle.Init.Request = DMA_REQUEST_USART1_RX;

	SetDmaProperty(dma->Context()._handle,
				   base::dma::PeripheralIncrement::DoNotIncrease,
				   base::dma::MemoryIncrement::Increase,
				   base::dma::PeripheralDataAlignment{1},
				   base::dma::MemoryDataAlignment{1},
				   base::dma::Priority::Medium);

	HAL_DMA_Init(&dma->Context()._handle);

	serial->Context()._uart_handle.hdmarx = &dma->Context()._handle;
	dma->Context()._handle.Parent = &serial->Context()._uart_handle;
}

/* #endregion */

int32_t base::dma::RemainingUntransmittedBytes(base::dma::IDma *dma)
{
	DMA_HandleTypeDef *handle = reinterpret_cast<DMA_HandleTypeDef *>(&dma->Context()._handle);
	return __HAL_DMA_GET_COUNTER(handle);
}

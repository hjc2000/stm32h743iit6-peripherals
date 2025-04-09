#pragma once
#include <base/define.h>
#include <bsp-interface/dma/IDmaChannel.h>
#include <hal.h>

namespace bsp
{
	class DmaStream :
		public bsp::dma::IDmaChannel
	{
	private:
		void LinkDmaToUartTx(UART_HandleTypeDef &uart);
		void LinkDmaToUartRx(UART_HandleTypeDef &uart);

	protected:
		DMA_HandleTypeDef _dma_handle{};

		void InitializeDmaProperty(bsp::dma::PeripheralIncrement peripheral_increment,
								   bsp::dma::MemoryIncrement const &memory_increment,
								   bsp::dma::PeripheralDataAlignment const &peripheral_data_alignment,
								   bsp::dma::MemoryDataAlignment const &memory_data_alignment,
								   bsp::dma::Priority priority,
								   std::string const &request);

	public:
		/// @brief 剩余的未传输的字节数。
		/// @note 将本次启动 DMA 所设置的目标传输字节数减去本属性，即可得到传输了多少个字节。
		/// @return
		int RemainingUntransmittedBytes() override
		{
			return __HAL_DMA_GET_COUNTER(&_dma_handle);
		}

		/// @brief 将 DMA 连接到父设备。
		/// @param parent
		void LinkDmaToParent(void *parent);
	};
} // namespace bsp

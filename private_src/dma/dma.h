#pragma once
#include "base/peripheral/IDma.h"
#include "hal.h"

class base::dma::DmaContext
{
public:
	DMA_HandleTypeDef _handle{};
};

namespace bsp
{
	namespace dma
	{
		void SetDmaProperty(DMA_HandleTypeDef &handle,
							base::dma::PeripheralIncrement peripheral_increment,
							base::dma::MemoryIncrement memory_increment,
							base::dma::PeripheralDataAlignment const &peripheral_data_alignment,
							base::dma::MemoryDataAlignment const &memory_data_alignment,
							base::dma::Priority priority);

		///
		/// @brief 获取 DMA 本次启动后剩余的未传输的字节数。
		///
		/// @note 将本次启动 DMA 所设置的目标传输字节数减去本属性，即可得到传输了多少个字节。
		///
		/// @param handle
		/// @return int
		///
		inline int GetRemainingUntransmittedBytes(DMA_HandleTypeDef &handle)
		{
			return __HAL_DMA_GET_COUNTER(&handle);
		}

		void EnableClock(DMA_HandleTypeDef &handle);

		class Dma1Stream0_ :
			public base::dma::IDma
		{
		private:
			Dma1Stream0_()
			{
				_context._handle.Instance = DMA1_Stream0;
			}

			base::dma::DmaContext _context{};

		public:
			static Dma1Stream0_ &Instance();

			virtual base::dma::DmaContext &Context() override
			{
				return _context;
			}
		};

		class Dma1Stream1_ :
			public base::dma::IDma
		{
		private:
			Dma1Stream1_()
			{
				_context._handle.Instance = DMA1_Stream1;
			}

			base::dma::DmaContext _context{};

		public:
			static Dma1Stream1_ &Instance();

			virtual base::dma::DmaContext &Context() override
			{
				return _context;
			}
		};

	} // namespace dma
} // namespace bsp

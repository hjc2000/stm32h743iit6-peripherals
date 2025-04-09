#pragma once
#include "hal.h"

namespace bsp
{
	namespace dma
	{
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

	} // namespace dma
} // namespace bsp

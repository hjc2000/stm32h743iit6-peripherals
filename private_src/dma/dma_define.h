#pragma once
#include "base/string/define.h"
#include "hal.h" // IWYU pragma: keep
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	namespace dma
	{
		constexpr uint32_t peripheral_align_byte_count_to_define_value(size_t align_byte_count)
		{
			switch (align_byte_count)
			{
			case 1:
				{
					return DMA_PDATAALIGN_BYTE;
				}
			case 2:
				{
					return DMA_PDATAALIGN_HALFWORD;
				}
			case 4:
				{
					return DMA_PDATAALIGN_WORD;
				}
			default:
				{
					throw std::invalid_argument{CODE_POS_STR + "非法对齐字节数。"};
				}
			}
		}

		constexpr uint32_t memory_align_byte_count_to_define_value(size_t align_byte_count)
		{
			switch (align_byte_count)
			{
			case 1:
				{
					return DMA_MDATAALIGN_BYTE;
				}
			case 2:
				{
					return DMA_MDATAALIGN_HALFWORD;
				}
			case 4:
				{
					return DMA_MDATAALIGN_WORD;
				}
			default:
				{
					throw std::invalid_argument{CODE_POS_STR + "非法对齐字节数。"};
				}
			}
		}

	} // namespace dma
} // namespace bsp

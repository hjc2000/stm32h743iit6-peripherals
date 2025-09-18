#pragma once
#include "base/embedded/sdram/parameter.h"
#include "base/string/define.h"
#include "hal.h"
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	namespace sdram
	{
		constexpr uint32_t row_bit_count_to_define(base::sdram::RowBitCount const &row_bit_count)
		{
			switch (row_bit_count.Value())
			{
			case 11:
				{
					return FMC_SDRAM_ROW_BITS_NUM_11;
				}
			case 12:
				{
					return FMC_SDRAM_ROW_BITS_NUM_12;
				}
			case 13:
				{
					return FMC_SDRAM_ROW_BITS_NUM_13;
				}
			default:
				{
					throw std::invalid_argument{CODE_POS_STR + "不支持的行地址位数。"};
				}
			}
		}

	} // namespace sdram

} // namespace bsp

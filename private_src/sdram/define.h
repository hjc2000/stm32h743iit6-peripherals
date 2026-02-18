#pragma once
#include "base/embedded/sdram/parameter.h"
#include "base/string/define.h"
#include "hal.h"
#include <cstdint>
#include <stdexcept>

namespace bsp::sdram
{
	constexpr uint32_t clock_source_divider_value_to_define(uint32_t divider)
	{
		switch (divider)
		{
		case 2:
			{
				return FMC_SDRAM_CLOCK_PERIOD_2;
			}
		case 3:
			{
				return FMC_SDRAM_CLOCK_PERIOD_3;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法分频系数。"};
			}
		}
	}

	constexpr uint32_t bank_count_to_define(base::sdram::BankCount const &bank_count)
	{
		switch (bank_count.Value())
		{
		case 2:
			{
				return FMC_SDRAM_INTERN_BANKS_NUM_2;
			}
		case 4:
			{
				return FMC_SDRAM_INTERN_BANKS_NUM_4;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "不支持的 BANK 数量。"};
			}
		}
	}

	///
	/// @brief 将行地址位数参数转换为 HAL 库的宏定义。
	///
	/// @param row_bit_count
	/// @return
	///
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

	///
	/// @brief 将列地址位数参数转换为 HAL 库的宏定义。
	///
	/// @param column_bit_count
	/// @return
	///
	constexpr uint32_t column_bit_count_to_define(base::sdram::ColumnBitCount const &column_bit_count)
	{
		switch (column_bit_count.Value())
		{
		case 8:
			{
				return FMC_SDRAM_COLUMN_BITS_NUM_8;
			}
		case 9:
			{
				return FMC_SDRAM_COLUMN_BITS_NUM_9;
			}
		case 10:
			{
				return FMC_SDRAM_COLUMN_BITS_NUM_10;
			}
		case 11:
			{
				return FMC_SDRAM_COLUMN_BITS_NUM_11;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "不支持的列地址位数。"};
			}
		}
	}

	///
	/// @brief 将数据宽度参数转换为 HAL 库的宏定义。
	///
	/// @param data_width
	/// @return
	///
	constexpr uint32_t data_width_to_define(base::sdram::DataWidth const &data_width)
	{
		switch (data_width.Value())
		{
		case 8:
			{
				return FMC_SDRAM_MEM_BUS_WIDTH_8;
			}
		case 16:
			{
				return FMC_SDRAM_MEM_BUS_WIDTH_16;
			}
		case 32:
			{
				return FMC_SDRAM_MEM_BUS_WIDTH_32;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "不支持的数据宽度。"};
			}
		}
	}

	///
	/// @brief
	///
	/// @param cas_latency
	/// @return
	///
	constexpr uint32_t cas_latency_value_to_define(uint32_t cas_latency)
	{
		switch (cas_latency)
		{
		case 1:
			{
				return FMC_SDRAM_CAS_LATENCY_1;
			}
		case 2:
			{
				return FMC_SDRAM_CAS_LATENCY_2;
			}
		case 3:
			{
				return FMC_SDRAM_CAS_LATENCY_3;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "不支持的 CASLatency."};
			}
		}
	}

} // namespace bsp::sdram

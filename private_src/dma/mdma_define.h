#pragma once
#include "base/string/define.h"
#include "hal.h" // IWYU pragma: keep
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	namespace mdma
	{
		constexpr uint32_t source_increase_size_to_define(size_t size, bool increase)
		{
			if (increase)
			{
				switch (size)
				{
				case 1:
					{
						return MDMA_SRC_INC_BYTE;
					}
				case 2:
					{
						return MDMA_SRC_INC_HALFWORD;
					}
				case 4:
					{
						return MDMA_SRC_INC_WORD;
					}
				case 8:
					{
						return MDMA_SRC_INC_DOUBLEWORD;
					}
				default:
					{
						throw std::invalid_argument{CODE_POS_STR + "非法递增大小。"};
					}
				}
			}
			else
			{
				switch (size)
				{
				case 1:
					{
						return MDMA_SRC_DEC_BYTE;
					}
				case 2:
					{
						return MDMA_SRC_DEC_HALFWORD;
					}
				case 4:
					{
						return MDMA_SRC_DEC_WORD;
					}
				case 8:
					{
						return MDMA_SRC_DEC_DOUBLEWORD;
					}
				default:
					{
						throw std::invalid_argument{CODE_POS_STR + "非法递增大小。"};
					}
				}
			}
		}

		constexpr uint32_t destination_increase_size_to_define(size_t size, bool increase)
		{
			if (increase)
			{
				switch (size)
				{
				case 1:
					{
						return MDMA_DEST_INC_BYTE;
					}
				case 2:
					{
						return MDMA_DEST_INC_HALFWORD;
					}
				case 4:
					{
						return MDMA_DEST_INC_WORD;
					}
				case 8:
					{
						return MDMA_DEST_INC_DOUBLEWORD;
					}
				default:
					{
						throw std::invalid_argument{CODE_POS_STR + "非法递增大小。"};
					}
				}
			}
			else
			{
				switch (size)
				{
				case 1:
					{
						return MDMA_DEST_DEC_BYTE;
					}
				case 2:
					{
						return MDMA_DEST_DEC_HALFWORD;
					}
				case 4:
					{
						return MDMA_DEST_DEC_WORD;
					}
				case 8:
					{
						return MDMA_DEST_DEC_DOUBLEWORD;
					}
				default:
					{
						throw std::invalid_argument{CODE_POS_STR + "非法递增大小。"};
					}
				}
			}
		}

		constexpr uint32_t source_data_size_to_define(size_t size)
		{
			switch (size)
			{
			case 1:
				{
					return MDMA_SRC_DATASIZE_BYTE;
				}
			case 2:
				{
					return MDMA_SRC_DATASIZE_HALFWORD;
				}
			case 4:
				{
					return MDMA_SRC_DATASIZE_WORD;
				}
			case 8:
				{
					return MDMA_SRC_DATASIZE_DOUBLEWORD;
				}
			default:
				{
					throw std::invalid_argument{CODE_POS_STR + "非法数据大小。"};
				}
			}
		}

		constexpr uint32_t destination_data_size_to_define(size_t size)
		{
			switch (size)
			{
			case 1:
				{
					return MDMA_DEST_DATASIZE_BYTE;
				}
			case 2:
				{
					return MDMA_DEST_DATASIZE_HALFWORD;
				}
			case 4:
				{
					return MDMA_DEST_DATASIZE_WORD;
				}
			case 8:
				{
					return MDMA_DEST_DATASIZE_DOUBLEWORD;
				}
			default:
				{
					throw std::invalid_argument{CODE_POS_STR + "非法数据大小。"};
				}
			}
		}

	} // namespace mdma
} // namespace bsp

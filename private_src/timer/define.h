#pragma once
#include "base/string/define.h"
#include "hal.h" // IWYU pragma: keep
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	constexpr uint32_t channel_id_to_channel_define(uint32_t channel_id)
	{
		switch (channel_id)
		{
		case 1:
			{
				return TIM_CHANNEL_1;
			}
		case 2:
			{
				return TIM_CHANNEL_2;
			}
		case 3:
			{
				return TIM_CHANNEL_3;
			}
		case 4:
			{
				return TIM_CHANNEL_4;
			}
		case 5:
			{
				return TIM_CHANNEL_5;
			}
		case 6:
			{
				return TIM_CHANNEL_6;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法通道 ID."};
			}
		}
	}

	constexpr uint32_t channel_define_to_channel_id(uint32_t define)
	{
		switch (define)
		{
		case TIM_CHANNEL_1:
			{
				return 1;
			}
		case TIM_CHANNEL_2:
			{
				return 2;
			}
		case TIM_CHANNEL_3:
			{
				return 3;
			}
		case TIM_CHANNEL_4:
			{
				return 4;
			}
		case TIM_CHANNEL_5:
			{
				return 5;
			}
		case TIM_CHANNEL_6:
			{
				return 6;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法通道定义。"};
			}
		}
	}

} // namespace bsp

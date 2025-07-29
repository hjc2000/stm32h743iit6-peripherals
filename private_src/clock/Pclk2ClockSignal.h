#pragma once
#include "base/string/define.h"
#include "base/unit/Hz.h"
#include "clock_source_handle.h"
#include "hal.h"
#include <stdexcept>

namespace bsp
{
	class Pclk2ClockSignal :
		public base::clock::clock_source_handle
	{
	public:
		/* #region Frequency */

		virtual base::unit::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetPCLK2Freq();
			return base::unit::MHz{base::unit::Hz{value}};
		}

		/* #endregion */

		/* #region Configure */

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override
		{
			auto it = channel_factor_map.find("in");
			if (it == channel_factor_map.end())
			{
				throw std::invalid_argument{CODE_POS_STR + "channel_factor_map 中没有 in 通道。"};
			}

			RCC_ClkInitTypeDef def{};
			def.ClockType = RCC_CLOCKTYPE_PCLK2;

			switch (it->second)
			{
			case 1:
				{
					def.APB2CLKDivider = RCC_APB2_DIV1;
					break;
				}
			case 2:
				{
					def.APB2CLKDivider = RCC_APB2_DIV2;
					break;
				}
			case 4:
				{
					def.APB2CLKDivider = RCC_APB2_DIV4;
					break;
				}
			case 8:
				{
					def.APB2CLKDivider = RCC_APB2_DIV8;
					break;
				}
			case 16:
				{
					def.APB2CLKDivider = RCC_APB2_DIV16;
					break;
				}
			default:
				{
					throw std::invalid_argument{"不支持此分频"};
				}
			}

			HAL_StatusTypeDef ret = HAL_RCC_ClockConfig(&def,
														FLASH_LATENCY_2);

			if (ret != HAL_StatusTypeDef::HAL_OK)
			{
				throw std::runtime_error{"时钟信号配置失败"};
			}
		}

		/* #endregion */
	};
} // namespace bsp

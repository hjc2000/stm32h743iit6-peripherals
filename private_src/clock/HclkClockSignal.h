#pragma once
#include "base/string/define.h"
#include "base/unit/Hz.h"
#include "clock_source_handle.h"
#include "hal.h"
#include <stdexcept>

namespace bsp
{
	class HclkClockSignal :
		public base::clock::clock_source_handle
	{
	public:
		/* #region Frequency */

		virtual base::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetHCLKFreq();
			return base::MHz{base::Hz{value}};
		}

		/* #endregion */

		/* #region Configure */

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override
		{
			auto it = channel_factor_map.find("in");
			if (it == channel_factor_map.end())
			{
				throw std::invalid_argument{CODE_POS_STR + "传入的 channel_factor_map 中没有 in 通道。"};
			}

			RCC_ClkInitTypeDef def{};
			def.ClockType = RCC_CLOCKTYPE_HCLK;
			switch (it->second)
			{
			case 1:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV1;
					break;
				}
			case 2:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV2;
					break;
				}
			case 4:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV4;
					break;
				}
			case 8:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV8;
					break;
				}
			case 16:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV16;
					break;
				}
			case 64:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV64;
					break;
				}
			case 128:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV128;
					break;
				}
			case 256:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV256;
					break;
				}
			case 512:
				{
					def.AHBCLKDivider = RCC_HCLK_DIV512;
					break;
				}
			default:
				{
					throw std::invalid_argument{"不支持的分频系数"};
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

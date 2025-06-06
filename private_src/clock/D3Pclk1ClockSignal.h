#pragma once
#include "base/unit/Hz.h"
#include "clock_source_handle.h"
#include "hal.h"

namespace bsp
{
	class D3Pclk1ClockSignal :
		public base::clock::clock_source_handle
	{
	private:
		inline static uint32_t _division_factor = 1;

	public:
		/* #region Frequency */

		virtual base::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetHCLKFreq() / _division_factor;
			return base::MHz{base::Hz{value}};
		}

		/* #endregion */

		/* #region Configure */

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override
		{
			auto it = channel_factor_map.find("in");
			if (it == channel_factor_map.end())
			{
				throw std::invalid_argument{CODE_POS_STR + "channel_factor_map 中没有 int 通道。"};
			}

			RCC_ClkInitTypeDef def{};
			def.ClockType = RCC_CLOCKTYPE_D3PCLK1;

			switch (it->second)
			{
			case 1:
				{
					def.APB4CLKDivider = RCC_APB4_DIV1;
					break;
				}
			case 2:
				{
					def.APB4CLKDivider = RCC_APB4_DIV2;
					break;
				}
			case 4:
				{
					def.APB4CLKDivider = RCC_APB4_DIV4;
					break;
				}
			case 8:
				{
					def.APB4CLKDivider = RCC_APB4_DIV8;
					break;
				}
			case 16:
				{
					def.APB4CLKDivider = RCC_APB4_DIV16;
					break;
				}
			default:
				{
					throw std::invalid_argument{"不支持此分频"};
				}
			}

			// 通过了上面的 switch 语句不抛出异常后才能更改 _division_factor
			_division_factor = it->second;

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

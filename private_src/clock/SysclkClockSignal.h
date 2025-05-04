#pragma once
#include "base/exception/NotSupportedException.h"
#include "base/string/define.h"
#include "base/unit/Hz.h"
#include "clock_source_handle.h"
#include "hal.h"
#include <stdexcept>

namespace bsp
{
	class SysclkClockSignal :
		public base::clock::clock_source_handle
	{
	public:
		/* #region Frequency */

		virtual base::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetSysClockFreq();
			return base::MHz{base::Hz{value}};
		}

		virtual base::MHz Frequency(std::string const &output_channel_name) const override
		{
			throw base::exception::NotSupportedException{};
		}

		/* #endregion */

		/* #region Configure */

		virtual void Configure() override
		{
			throw base::exception::NotSupportedException{};
		}

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override
		{
			throw base::exception::NotSupportedException{};
		}

		virtual void Configure(std::string const &input_channel_name,
							   std::map<std::string, uint32_t> const &channel_factor_map) override
		{
			RCC_ClkInitTypeDef def{};
			def.ClockType = RCC_CLOCKTYPE_SYSCLK;

			if (input_channel_name == "hse")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
			}
			else if (input_channel_name == "hsi")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
			}
			else if (input_channel_name == "csi")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_CSI;
			}
			else if (input_channel_name == "pll")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			}
			else
			{
				throw std::invalid_argument{"不支持此时钟源"};
			}

			auto it = channel_factor_map.find("out");
			if (it == channel_factor_map.end())
			{
				throw std::invalid_argument{CODE_POS_STR + "channel_factor_map 中没有 out 信道。"};
			}

			switch (it->second)
			{
			case 1:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV1;
					break;
				}
			case 2:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV2;
					break;
				}
			case 4:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV4;
					break;
				}
			case 8:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV8;
					break;
				}
			case 16:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV16;
					break;
				}
			case 64:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV64;
					break;
				}
			case 128:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV128;
					break;
				}
			case 256:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV256;
					break;
				}
			case 512:
				{
					def.SYSCLKDivider = RCC_SYSCLK_DIV512;
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

		virtual void ConfigureAsBypassMode(base::MHz const &bypass_input_frequency) override
		{
			throw base::exception::NotSupportedException{};
		}

		virtual void TurnOff() override
		{
			throw base::exception::NotSupportedException{};
		}
	};
} // namespace bsp

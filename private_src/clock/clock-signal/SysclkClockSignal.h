#pragma once
#include "base/define.h"
#include "base/unit/Hz.h"
#include "bsp-interface/clock/IClockSignal.h"
#include "hal.h"

namespace bsp
{
	class SysclkClockSignal :
		public bsp::IClockSignal
	{
	public:
		static_function SysclkClockSignal &Instance()
		{
			static SysclkClockSignal o{};
			return o;
		}

		/// @brief 时钟信号的名称。
		/// @return
		virtual std::string Name() const override
		{
			return "sysclk";
		}

		/// @brief 时钟信号的频率
		/// @return
		virtual base::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetSysClockFreq();
			return base::MHz{base::Hz{value}};
		}

		/// @brief 打开时钟信号。
		/// @param output_division_factor 输出分频系数。
		/// @param clock_source 时钟源。像 stm32 的系统时钟 sysclk，是时钟源后的第一个时钟信号，输入端连接着
		/// 各个时钟源，输出端供给各个子时钟信号。本参数用来选择类似 sysclk 这种时钟信号的时钟源。
		virtual void Open(bsp::IClockSignal_OutputDivisionFactor const &output_division_factor,
						  IClockSignal_ClockSource const &clock_source) override
		{
			RCC_ClkInitTypeDef def{};
			def.ClockType = RCC_CLOCKTYPE_SYSCLK;

			if (clock_source.Value() == "hse")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
			}
			else if (clock_source.Value() == "hsi")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
			}
			else if (clock_source.Value() == "csi")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_CSI;
			}
			else if (clock_source.Value() == "pll")
			{
				def.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			}
			else
			{
				throw std::invalid_argument{"不支持此时钟源"};
			}

			switch (output_division_factor.Value())
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
	};
} // namespace bsp

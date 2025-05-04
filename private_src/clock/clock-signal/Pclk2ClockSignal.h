#pragma once
#include "base/define.h"
#include "base/unit/Hz.h"
#include "bsp-interface/clock/IClockSignal.h"
#include "hal.h"

namespace bsp
{
	class Pclk2ClockSignal :
		public bsp::IClockSignal
	{
	public:
		static_function Pclk2ClockSignal &Instance()
		{
			static Pclk2ClockSignal o{};
			return o;
		}

		/// @brief 时钟信号的名称。
		/// @return
		virtual std::string Name() const override
		{
			return "pclk2";
		}

		/// @brief 时钟信号的频率
		/// @return
		virtual base::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetPCLK2Freq();
			return base::MHz{base::Hz{value}};
		}

		/// @brief 打开时钟信号。
		/// @note 有的时钟信号只有输入分频，没有输出分频，就使用本重载。
		/// @param input_division_factor
		virtual void Open(bsp::IClockSignal_InputDivisionFactor const &input_division_factor) override
		{
			RCC_ClkInitTypeDef def{};
			def.ClockType = RCC_CLOCKTYPE_PCLK2;

			switch (input_division_factor.Value())
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
	};
} // namespace bsp

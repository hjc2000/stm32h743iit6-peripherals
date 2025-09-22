#pragma once
#include "base/unit/MHz.h"
#include "clock_source_handle.h"
#include "hal.h"

namespace bsp
{
	class PllClockSource final :
		public base::clock::clock_source_handle
	{
	private:
		inline static bool _opened = false;
		inline static base::unit::MHz _p_freq;
		inline static base::unit::MHz _q_freq;
		inline static base::unit::MHz _r_freq;

	public:
		virtual base::unit::MHz Frequency(std::string const &output_channel_name) const override;

		virtual void Configure(std::string const &input_channel_name,
							   std::map<std::string, uint32_t> const &channel_factor_map) override;

		virtual void TurnOff() override
		{
			RCC_OscInitTypeDef def{};
			def.OscillatorType = RCC_OSCILLATORTYPE_NONE;
			def.PLL.PLLState = RCC_PLL_OFF;
			HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
			if (result != HAL_StatusTypeDef::HAL_OK)
			{
				throw std::runtime_error{"关闭 PLL 失败。"};
			}

			_opened = false;
		}
	};

} // namespace bsp

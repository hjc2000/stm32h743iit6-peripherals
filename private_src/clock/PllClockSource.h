#pragma once
#include "base/unit/MHz.h"
#include "clock_source_handle.h"
#include "hal.h"
#include <cstdint>
#include <string>

namespace bsp
{
	class PllClockSource final :
		public base::clock::clock_source_handle
	{
	private:
		struct Factors
		{
			uint32_t _m{};
			uint32_t _n{};
			uint32_t _p{};
			uint32_t _q{};
			uint32_t _r{};
		};

		inline static bool _configured = false;
		inline static base::unit::MHz _p_freq;
		inline static base::unit::MHz _q_freq;
		inline static base::unit::MHz _r_freq;

		static uint32_t input_channel_name_to_define_value(std::string const &input_channel_name);

		static Factors get_factors(std::map<std::string, uint32_t> const &channel_factor_map);

		static base::unit::MHz get_input_frequency(std::string const &input_channel_name);

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

			_configured = false;
		}
	};

} // namespace bsp

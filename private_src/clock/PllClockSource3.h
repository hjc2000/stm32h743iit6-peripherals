#pragma once
#include "base/unit/MHz.h"
#include "clock_source_handle.h"
#include "hal.h" // IWYU pragma: keep
#include <cstdint>
#include <string>

namespace bsp
{
	class PllClockSource3 final :
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

		static Factors get_factors(std::map<std::string, uint32_t> const &channel_factor_map);

		static base::unit::MHz get_input_frequency();

		///
		/// @brief
		///
		/// @param m_channel_output_frequency m 通道的输出频率。
		/// @return
		///
		static uint32_t calculate_pll_range(base::unit::MHz const &m_channel_output_frequency);

	public:
		virtual base::unit::MHz Frequency(std::string const &output_channel_name) const override;

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override;

		virtual void TurnOff() override
		{
		}
	};

} // namespace bsp

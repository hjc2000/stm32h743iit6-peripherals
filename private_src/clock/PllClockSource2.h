#pragma once
#include "base/SingletonProvider.h"
#include "base/unit/MHz.h"
#include "clock_source_handle.h"
#include "hal.h" // IWYU pragma: keep
#include <cstdint>
#include <string>

namespace bsp
{
	class PllClockSource2 final :
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

		struct SingletonContext
		{
			bool _configured = false;

			base::unit::MHz _p_freq;
			base::unit::MHz _q_freq;
			base::unit::MHz _r_freq;
		};

		inline static base::SingletonProvider<SingletonContext> _singleton_context_provider{};

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

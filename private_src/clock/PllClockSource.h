#pragma once
#include "base/SingletonProvider.h"
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

		///
		/// @brief 需要是单例的上下文保存到这里。
		///
		struct SingletonContext
		{
			bool _configured = false;

			base::unit::MHz _p_freq;
			base::unit::MHz _q_freq;
			base::unit::MHz _r_freq;

			std::string _clock_source_name;
		};

		inline static base::SingletonProvider<SingletonContext> _singleton_context_provider{};

		static uint32_t input_channel_name_to_define_value(std::string const &input_channel_name);

		static Factors get_factors(std::map<std::string, uint32_t> const &channel_factor_map);

		static base::unit::MHz get_input_frequency(std::string const &input_channel_name);

		///
		/// @brief
		///
		/// @param m_channel_output_frequency m 通道的输出频率。
		/// @return
		///
		static uint32_t calculate_pll_range(base::unit::MHz const &m_channel_output_frequency);

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

			_singleton_context_provider.Instance()._configured = false;
		}

		///
		/// @brief 获取 PLL 的时钟源的名称。
		///
		/// @return
		///
		std::string ClockSourceName() const
		{
			return _singleton_context_provider.Instance()._clock_source_name;
		}
	};

} // namespace bsp

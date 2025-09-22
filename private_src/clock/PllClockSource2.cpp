#include "PllClockSource2.h" // IWYU pragma: keep
#include "base/embedded/clock/ClockSource.h"
#include "base/string/define.h"
#include "PllClockSource.h"
#include <cstdint>
#include <string>

/* #region 静态的配置帮助函数 */

bsp::PllClockSource2::Factors bsp::PllClockSource2::get_factors(std::map<std::string, uint32_t> const &channel_factor_map)
{
	Factors ret{};

	{
		auto it = channel_factor_map.find("m");
		if (it == channel_factor_map.end())
		{
			throw std::invalid_argument{CODE_POS_STR + "缺少 m 因子。"};
		}

		ret._m = it->second;
	}

	{
		auto it = channel_factor_map.find("n");
		if (it == channel_factor_map.end())
		{
			throw std::invalid_argument{CODE_POS_STR + "缺少 n 因子。"};
		}

		ret._n = it->second;
	}

	{
		auto it = channel_factor_map.find("p");
		if (it == channel_factor_map.end())
		{
			throw std::invalid_argument{CODE_POS_STR + "缺少 p 因子。"};
		}

		ret._p = it->second;
	}

	{
		auto it = channel_factor_map.find("q");
		if (it == channel_factor_map.end())
		{
			throw std::invalid_argument{CODE_POS_STR + "缺少 q 因子。"};
		}

		ret._q = it->second;
	}

	{
		auto it = channel_factor_map.find("r");
		if (it == channel_factor_map.end())
		{
			throw std::invalid_argument{CODE_POS_STR + "缺少 r 因子。"};
		}

		ret._r = it->second;
	}

	return ret;
}

base::unit::MHz bsp::PllClockSource2::get_input_frequency()
{
	bsp::PllClockSource pll{};
	std::string input_channel_name = pll.ClockSourceName();

	if (input_channel_name == "hse")
	{
		base::clock::ClockSource clock_source{"hse"};
		base::unit::MHz ret = clock_source.Frequency();
		return ret;
	}

	if (input_channel_name == "hsi")
	{
		base::clock::ClockSource clock_source{"hsi"};
		base::unit::MHz ret = clock_source.Frequency();
		return ret;
	}

	if (input_channel_name == "csi")
	{
		base::clock::ClockSource clock_source{"csi"};
		base::unit::MHz ret = clock_source.Frequency();
		return ret;
	}

	throw std::invalid_argument{CODE_POS_STR + "非法输入通道名。"};
}

uint32_t bsp::PllClockSource2::calculate_pll_range(base::unit::MHz const &m_channel_output_frequency)
{
	if (m_channel_output_frequency < base::unit::MHz{2})
	{
		return RCC_PLL2VCIRANGE_0;
	}

	if (m_channel_output_frequency >= base::unit::MHz{2} && m_channel_output_frequency < base::unit::MHz{4})
	{
		return RCC_PLL2VCIRANGE_1;
	}

	if (m_channel_output_frequency >= base::unit::MHz{4} && m_channel_output_frequency < base::unit::MHz{8})
	{
		return RCC_PLL2VCIRANGE_2;
	}

	return RCC_PLL2VCIRANGE_3;
}

/* #endregion */

base::unit::MHz bsp::PllClockSource2::Frequency(std::string const &output_channel_name) const
{
	if (!_configured)
	{
		throw std::runtime_error{CODE_POS_STR + "必须先通过本类配置后才能查看频率。"};
	}

	if (output_channel_name == "p")
	{
		return _p_freq;
	}

	if (output_channel_name == "q")
	{
		return _q_freq;
	}

	if (output_channel_name == "r")
	{
		return _r_freq;
	}

	throw std::invalid_argument{CODE_POS_STR + "非法输出通道。"};
}

void bsp::PllClockSource2::Configure(std::map<std::string, uint32_t> const &channel_factor_map)
{
	Factors factors = get_factors(channel_factor_map);
	base::unit::MHz input_frequency = get_input_frequency();
	uint32_t pll_range = calculate_pll_range(input_frequency / factors._m);

	RCC_PeriphCLKInitTypeDef def{};
	def.PeriphClockSelection = RCC_PERIPHCLK_PLL2_DIVP | RCC_PERIPHCLK_PLL2_DIVQ | RCC_PERIPHCLK_PLL2_DIVR;
	def.PLL2.PLL2M = factors._m;
	def.PLL2.PLL2N = factors._n;
	def.PLL2.PLL2P = factors._p;
	def.PLL2.PLL2Q = factors._q;
	def.PLL2.PLL2R = factors._r;
	def.PLL2.PLL2RGE = pll_range;
	def.PLL2.PLL2FRACN = 0;

	HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "配置 PLL 失败。"};
	}

	// 打开后，记录各个输出通道的频率
	_p_freq = input_frequency / factors._m * factors._n / factors._p;
	_q_freq = input_frequency / factors._m * factors._n / factors._q;
	_r_freq = input_frequency / factors._m * factors._n / factors._r;

	_configured = true;
}

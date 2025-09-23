#include "PllClockSource.h" // IWYU pragma: keep
#include "base/embedded/clock/ClockSource.h"
#include "base/string/define.h"
#include <cstdint>

/* #region 静态的配置帮助函数 */

uint32_t bsp::PllClockSource::input_channel_name_to_define_value(std::string const &input_channel_name)
{
	if (input_channel_name == "hse")
	{
		return RCC_PLLSOURCE_HSE;
	}
	else if (input_channel_name == "hsi")
	{
		return RCC_PLLSOURCE_HSI;
	}
	else if (input_channel_name == "csi")
	{
		return RCC_PLLSOURCE_CSI;
	}
	else
	{
		throw std::invalid_argument{CODE_POS_STR + "非法输入通道名。"};
	}
}

bsp::PllClockSource::Factors bsp::PllClockSource::get_factors(std::map<std::string, uint32_t> const &channel_factor_map)
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

base::unit::MHz bsp::PllClockSource::get_input_frequency(std::string const &input_channel_name)
{
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

uint32_t bsp::PllClockSource::calculate_pll_range(base::unit::MHz const &m_channel_output_frequency)
{
	if (m_channel_output_frequency < base::unit::MHz{2})
	{
		return RCC_PLL1VCIRANGE_0;
	}

	if (m_channel_output_frequency >= base::unit::MHz{2} && m_channel_output_frequency < base::unit::MHz{4})
	{
		return RCC_PLL1VCIRANGE_1;
	}

	if (m_channel_output_frequency >= base::unit::MHz{4} && m_channel_output_frequency < base::unit::MHz{8})
	{
		return RCC_PLL1VCIRANGE_2;
	}

	return RCC_PLL1VCIRANGE_3;
}

/* #endregion */

base::unit::MHz bsp::PllClockSource::Frequency(std::string const &output_channel_name) const
{
	if (!_singleton_context_provider.Instance()._configured)
	{
		throw std::runtime_error{CODE_POS_STR + "必须先通过本类配置后才能查看频率。"};
	}

	if (output_channel_name == "p")
	{
		return _singleton_context_provider.Instance()._p_freq;
	}

	if (output_channel_name == "q")
	{
		return _singleton_context_provider.Instance()._q_freq;
	}

	if (output_channel_name == "r")
	{
		return _singleton_context_provider.Instance()._r_freq;
	}

	throw std::invalid_argument{CODE_POS_STR + "非法输出通道。"};
}

void bsp::PllClockSource::Configure(std::string const &input_channel_name,
									std::map<std::string, uint32_t> const &channel_factor_map)
{
	_singleton_context_provider.Instance()._clock_source_name = input_channel_name;
	Factors factors = get_factors(channel_factor_map);
	base::unit::MHz input_frequency = get_input_frequency(input_channel_name);
	uint32_t pll_range = calculate_pll_range(input_frequency / factors._m);

	RCC_OscInitTypeDef def{};
	def.OscillatorType = RCC_OSCILLATORTYPE_NONE;
	def.PLL.PLLState = RCC_PLL_ON;
	def.PLL.PLLSource = input_channel_name_to_define_value(input_channel_name);
	def.PLL.PLLRGE = pll_range;
	def.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	def.PLL.PLLM = factors._m;
	def.PLL.PLLN = factors._n;
	def.PLL.PLLP = factors._p;
	def.PLL.PLLQ = factors._q;
	def.PLL.PLLR = factors._r;

	HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "配置 PLL 失败。"};
	}

	// 打开后，记录各个输出通道的频率
	_singleton_context_provider.Instance()._p_freq = input_frequency / factors._m * factors._n / factors._p;
	_singleton_context_provider.Instance()._q_freq = input_frequency / factors._m * factors._n / factors._q;
	_singleton_context_provider.Instance()._r_freq = input_frequency / factors._m * factors._n / factors._r;

	_singleton_context_provider.Instance()._configured = true;
}

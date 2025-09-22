#include "PllClockSource.h" // IWYU pragma: keep
#include "base/embedded/clock/ClockSource.h"
#include "base/string/define.h"

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

base::unit::MHz bsp::PllClockSource::Frequency(std::string const &output_channel_name) const
{
	if (!_opened)
	{
		throw std::runtime_error{"pll 还未打开，无法查看频率"};
	}

	if (output_channel_name == "p")
	{
		return _p_freq;
	}
	else if (output_channel_name == "q")
	{
		return _q_freq;
	}
	else if (output_channel_name == "r")
	{
		return _r_freq;
	}
	else
	{
		throw std::invalid_argument{"没有该输出通道"};
	}
}

void bsp::PllClockSource::Configure(std::string const &input_channel_name,
									std::map<std::string, uint32_t> const &channel_factor_map)
{
	/* #region m,n,p,q,r */

	int m = 1;
	{
		auto it = channel_factor_map.find("m");
		if (it != channel_factor_map.end())
		{
			m = it->second;
		}
	}

	int n = 1;
	{
		auto it = channel_factor_map.find("n");
		if (it != channel_factor_map.end())
		{
			n = it->second;
		}
	}

	int p = 1;
	{
		auto it = channel_factor_map.find("p");
		if (it != channel_factor_map.end())
		{
			p = it->second;
		}
	}

	int q = 1;
	{
		auto it = channel_factor_map.find("q");
		if (it != channel_factor_map.end())
		{
			q = it->second;
		}
	}

	int r = 1;
	{
		auto it = channel_factor_map.find("r");
		if (it != channel_factor_map.end())
		{
			r = it->second;
		}
	}

	/* #endregion */

	base::unit::MHz input_freq;
	if (input_channel_name == "hse")
	{
		base::clock::ClockSource hse{"hse"};
		input_freq = hse.Frequency();
	}
	else if (input_channel_name == "hsi")
	{
		throw std::invalid_argument{"不支持该输入通道"};
	}
	else if (input_channel_name == "csi")
	{
		throw std::invalid_argument{"不支持该输入通道"};
	}
	else
	{
		throw std::invalid_argument{"不支持该输入通道"};
	}

	/* #region pll_range */

	int pll_range = RCC_PLL1VCIRANGE_2;
	{
		// 经过 m 分频系数分频后输入锁相环，这里需要根据输入锁相环的频率所处的范围来设置参数。
		base::unit::MHz divided_input_freq = input_freq / m;
		if (divided_input_freq < base::unit::MHz{2})
		{
			pll_range = RCC_PLL1VCIRANGE_0;
		}
		else if (divided_input_freq >= base::unit::MHz{2} && divided_input_freq < base::unit::MHz{4})
		{
			pll_range = RCC_PLL1VCIRANGE_1;
		}
		else if (divided_input_freq >= base::unit::MHz{4} && divided_input_freq < base::unit::MHz{8})
		{
			pll_range = RCC_PLL1VCIRANGE_2;
		}
		else
		{
			pll_range = RCC_PLL1VCIRANGE_3;
		}
	}

	/* #endregion */

	RCC_OscInitTypeDef def{};
	def.OscillatorType = RCC_OSCILLATORTYPE_NONE;
	def.PLL.PLLState = RCC_PLL_ON;
	def.PLL.PLLSource = input_channel_name_to_define_value(input_channel_name);
	def.PLL.PLLRGE = pll_range;
	def.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	def.PLL.PLLM = m;
	def.PLL.PLLN = n;
	def.PLL.PLLP = p;
	def.PLL.PLLQ = q;
	def.PLL.PLLR = r;

	HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "配置 PLL 失败。"};
	}

	// 打开后，记录各个输出通道的频率
	_p_freq = input_freq / m * n / p;
	_q_freq = input_freq / m * n / q;
	_r_freq = input_freq / m * n / r;

	_opened = true;
}

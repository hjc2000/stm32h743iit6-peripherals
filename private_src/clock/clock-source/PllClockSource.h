#pragma once
#include "base/embedded/clock/ClockSource.h"
#include "base/exception/NotSupportedException.h"
#include "base/unit/MHz.h"
#include "clock_source_handle.h"
#include "hal.h"

namespace bsp
{
	class PllClockSource :
		public base::clock::clock_source_handle
	{
	private:
		bool _opened = false;
		base::MHz _p_freq;
		base::MHz _q_freq;
		base::MHz _r_freq;

	public:
		/* #region Frequency */

		virtual base::MHz Frequency() const override
		{
			throw base::exception::NotSupportedException{};
		}

		virtual base::MHz Frequency(std::string const &output_channel_name) const override
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

		/* #endregion */

		virtual void Configure() override
		{
			throw base::exception::NotSupportedException{};
		}

		virtual void Configure(std::string const &input_channel_name,
							   std::map<std::string, uint32_t> const &channel_factor_map) override
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

			/* #region pll_source */

			uint32_t pll_source = RCC_PLLSOURCE_HSE;
			if (input_channel_name == "hse")
			{
				pll_source = RCC_PLLSOURCE_HSE;
			}
			else if (input_channel_name == "hsi")
			{
				pll_source = RCC_PLLSOURCE_HSI;
			}
			else if (input_channel_name == "csi")
			{
				pll_source = RCC_PLLSOURCE_CSI;
			}
			else
			{
				throw std::invalid_argument{"不支持该输入通道"};
			}

			/* #endregion */

			/* #region pll_range */

			base::MHz input_freq;
			int pll_range = RCC_PLL1VCIRANGE_2;
			if (input_channel_name == "hse")
			{
				base::clock::ClockSource hse{"hse"};
				input_freq = hse.Frequency();

				// 经过 m 分频系数分频后输入锁相环，这里需要根据输入锁相环的频率所处的范围来设置参数。
				base::MHz divided_input_freq = input_freq / m;
				if (divided_input_freq < base::MHz{2})
				{
					pll_range = RCC_PLL1VCIRANGE_0;
				}
				else if (divided_input_freq >= base::MHz{2} && divided_input_freq < base::MHz{4})
				{
					pll_range = RCC_PLL1VCIRANGE_1;
				}
				else if (divided_input_freq >= base::MHz{4} && divided_input_freq < base::MHz{8})
				{
					pll_range = RCC_PLL1VCIRANGE_2;
				}
				else
				{
					pll_range = RCC_PLL1VCIRANGE_3;
				}
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

			/* #endregion */

			RCC_OscInitTypeDef def{};
			def.OscillatorType = RCC_OSCILLATORTYPE_NONE;
			def.PLL.PLLState = RCC_PLL_ON;
			def.PLL.PLLSource = pll_source;
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
				throw std::runtime_error{"打开 PLL 失败。"};
			}

			// 打开后，记录各个输出通道的频率
			_p_freq = input_freq / m * n / p;
			_q_freq = input_freq / m * n / q;
			_r_freq = input_freq / m * n / r;

			_opened = true;
		}

		virtual void ConfigureAsBypassMode(base::MHz const &bypass_input_frequency) override
		{
			throw base::exception::NotSupportedException{};
		}

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

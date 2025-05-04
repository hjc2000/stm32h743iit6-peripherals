#pragma once
#include "base/define.h"
#include "base/embedded/clock/ClockSource.h"
#include "base/unit/MHz.h"
#include "bsp-interface/clock/IClockSource.h"
#include "hal.h"

namespace bsp
{
	class PllClockSource :
		public bsp::IClockSource
	{
	private:
		bool _opened = false;
		IClockSource_State _state = IClockSource_State::Off;
		base::MHz _p_freq;
		base::MHz _q_freq;
		base::MHz _r_freq;

	public:
		static_function PllClockSource &Instance()
		{
			static PllClockSource o{};
			return o;
		}

		/// @brief 本时钟源的名称。
		/// @return
		std::string Name() const override
		{
			return "pll";
		}

		/// @brief 用户自己决定输入通道和各个分频、倍频系数。
		/// @param input_channel_name 输入通道名。
		/// @param factor 分频、倍频系数都放在这个字典里，使用不同的名称来区分是什么。
		void Open(std::string const &input_channel_name,
				  base::IDictionary<std::string, int> const &factors) override
		{
			/* #region m,n,p,q,r */

			int m = 1;

			{
				int const *ptr = factors.Find("m");
				if (ptr != nullptr)
				{
					m = *ptr;
				}
			}

			int n = 1;

			{
				int const *ptr = factors.Find("n");
				if (ptr != nullptr)
				{
					n = *ptr;
				}
			}

			int p = 1;

			{
				int const *ptr = factors.Find("p");
				if (ptr != nullptr)
				{
					p = *ptr;
				}
			}

			int q = 1;

			{
				int const *ptr = factors.Find("q");
				if (ptr != nullptr)
				{
					q = *ptr;
				}
			}

			int r = 1;

			{
				int const *ptr = factors.Find("r");
				if (ptr != nullptr)
				{
					r = *ptr;
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

		/// @brief 关闭时钟源。
		void Close() override
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

		/// @brief 本时钟源当前的状态。
		/// @return
		IClockSource_State State() const override
		{
			return _state;
		}

		/// @brief 查看某一个输出通道的频率。有的时钟源会有多个输出通道。
		/// @param output_channel_name 输出通道名。
		/// @return
		base::MHz Frequency(std::string const &output_channel_name) const override
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
	};
} // namespace bsp

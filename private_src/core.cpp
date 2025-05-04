#include "base/embedded/core.h"
#include "base/embedded/cache/cache.h"
#include "base/embedded/clock/ClockSource.h"
#include "bsp-interface/di/clock.h"
#include "hal.h"
#include <cstdint>

namespace
{
	void InitializeClock()
	{
		/* #region hse */
		{
			base::clock::ClockSource hse{"hse"};
			hse.Configure();
		}
		/* #endregion */

		/* #region pll */
		{
			base::clock::ClockSource pll{"pll"};

			///
			/// HSE 接的是 25MHz 的晶振。
			///		@note m 是对 PLL 输入时钟的分频。25MHz 的晶振分频后是 5MHz.
			///
			/// 	@note n 是对经过 m 分频后的信号进行倍频。5MHz 被 192 倍频后是 960MHz.
			///
			/// 	@note p,q,r 是对 n 的输出进行分频。这里是 2 分频，于是得到的都是 480MHz 的时钟信号。
			/// 	其中 p 的输出作为系统时钟的输入。于是系统时钟频率是 480MHz. 系统时钟直接输入给 CPU.
			///
			std::map<std::string, uint32_t> factors{
				{"m", 5},
				{"n", 192},
				{"p", 2},
				{"q", 2},
				{"r", 2},
			};

			pll.Configure("hse", factors);
		}
		/* #endregion */

		/* #region hclk */
		{
			base::clock::ClockSource hclk{"hclk"};

			std::map<std::string, uint32_t> factors{
				{"in", 2},
			};

			hclk.Configure(factors);
		}
		/* #endregion */

		/* #region pclk1 */
		{
			base::clock::ClockSource pclk1{"pclk1"};

			std::map<std::string, uint32_t> factors{
				{"in", 2},
			};

			pclk1.Configure(factors);
		}
		/* #endregion */

		/* #region pclk2 */
		{
			base::clock::ClockSource pclk2{"pclk2"};

			std::map<std::string, uint32_t> factors{
				{"in", 2},
			};

			pclk2.Configure(factors);
		}
		/* #endregion */

		bsp::di::clock::ClockSignalCollection().Get("d1pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("d3pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});

		/* #region sysclk */
		{
			base::clock::ClockSource sysclk{"sysclk"};

			std::map<std::string, uint32_t> factors{
				{"out", 1},
			};

			sysclk.Configure("pll", factors);
		}
		/* #endregion */
	}

} // namespace

void base::core::initialize()
{
	HAL_Init();

	MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY)
	{
	}

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	HAL_EnableCompensationCell();
	base::cache::enable();

	InitializeClock();
}

void base::core::reset()
{
	HAL_NVIC_SystemReset();
}

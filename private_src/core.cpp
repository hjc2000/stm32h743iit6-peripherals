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
		base::clock::ClockSource hse{"hse"};
		hse.Configure();

		{
			std::map<std::string, uint32_t> factors;

			/**
			 * HSE 接的是 25MHz 的晶振。
			 * 		@li m 是对 PLL 输入时钟的分频。25MHz 的晶振分频后是 5MHz.
			 * 		@li n 是对经过 m 分频后的信号进行倍频。5MHz 被 192 倍频后是 960MHz.
			 * 		@li p,q,r 是对 n 的输出进行分频。这里是 2 分频，于是得到的都是 480MHz 的时钟信号。
			 * 			其中 p 的输出作为系统时钟的输入。于是系统时钟频率是 480MHz. 系统时钟直接输入给 CPU.
			 */
			factors["m"] = 5;
			factors["n"] = 192;
			factors["p"] = 2;
			factors["q"] = 2;
			factors["r"] = 2;
			base::clock::ClockSource pll{"pll"};
			pll.Configure("hse", factors);
		}

		bsp::di::clock::ClockSignalCollection().Get("hclk")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("pclk2")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("d1pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});
		bsp::di::clock::ClockSignalCollection().Get("d3pclk1")->Open(bsp::IClockSignal_InputDivisionFactor{2});

		bsp::di::clock::ClockSignalCollection().Get("sysclk")->Open(bsp::IClockSignal_OutputDivisionFactor{1},
																	bsp::IClockSignal_ClockSource{"pll"});
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

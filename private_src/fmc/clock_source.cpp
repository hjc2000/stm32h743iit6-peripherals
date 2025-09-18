#include "clock_source.h" // IWYU pragma: keep
#include "base/string/define.h"
#include "hal.h"
#include <stdexcept>

void bsp::config_fmc_clock_source(std::string const &clock_source_name)
{
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct{};

	if (clock_source_name == "hclk3")
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		return;
	}

	if (clock_source_name == "pll1_q")
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		return;
	}

	if (clock_source_name == "pll2_r")
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		return;
	}

	if (clock_source_name == "per_ck")
	{
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_CLKP;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		return;
	}

	throw std::invalid_argument{CODE_POS_STR + "无效的时钟源名称。"};
}

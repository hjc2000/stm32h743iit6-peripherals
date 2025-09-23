#include "FmcClock.h" // IWYU pragma: keep
#include "base/embedded/clock/ClockSource.h"
#include "base/string/define.h"
#include "hal.h"
#include "PllClockSource2.h"
#include <stdexcept>

base::unit::MHz bsp::FmcClock::Frequency() const
{
	if (!_singleton_context_provider.Instance()._configured)
	{
		throw std::runtime_error{CODE_POS_STR + "需要先通过本类配置后才能查询频率。"};
	}

	switch (_singleton_context_provider.Instance()._input_channel)
	{
	case InputChannel::HCLK3:
		{
			base::clock::ClockSource clock_source{"hclk"};
			base::unit::MHz clock_source_freq = clock_source.Frequency();
			return clock_source_freq;
		}
	case InputChannel::PLL1_Q:
		{
			base::clock::ClockSource clock_source{"pll"};
			base::unit::MHz clock_source_freq = clock_source.Frequency("q");
			return clock_source_freq;
		}
	case InputChannel::PLL2_R:
		{
			base::clock::ClockSource clock_source{"pll2"};
			base::unit::MHz clock_source_freq = clock_source.Frequency("r");
			return clock_source_freq;
		}
	case InputChannel::PER_CK:
		{
			throw std::runtime_error{CODE_POS_STR + "暂不支持。"};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "无效的时钟源输入通道。"};
		}
	}
}

void bsp::FmcClock::Configure(std::string const &input_channel_name)
{
	if (input_channel_name == "hclk3")
	{
		RCC_PeriphCLKInitTypeDef PeriphClkInitStruct{};
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		_singleton_context_provider.Instance()._input_channel = InputChannel::HCLK3;
		_singleton_context_provider.Instance()._configured = true;
		return;
	}

	if (input_channel_name == "pll1_q")
	{
		RCC_PeriphCLKInitTypeDef PeriphClkInitStruct{};
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		_singleton_context_provider.Instance()._input_channel = InputChannel::PLL1_Q;
		_singleton_context_provider.Instance()._configured = true;
		return;
	}

	if (input_channel_name == "pll2_r")
	{
		bsp::PllClockSource2 pll2_clock_source{};
		RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = pll2_clock_source.HalInitializationConfiguration();
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL2;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		_singleton_context_provider.Instance()._input_channel = InputChannel::PLL2_R;
		_singleton_context_provider.Instance()._configured = true;
		return;
	}

	if (input_channel_name == "per_ck")
	{
		RCC_PeriphCLKInitTypeDef PeriphClkInitStruct{};
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
		PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_CLKP;
		HAL_StatusTypeDef result = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
		if (result != HAL_OK)
		{
			throw std::runtime_error{CODE_POS_STR + "配置 FMC 时钟源失败。"};
		}

		_singleton_context_provider.Instance()._input_channel = InputChannel::PER_CK;
		_singleton_context_provider.Instance()._configured = true;
		return;
	}

	throw std::invalid_argument{CODE_POS_STR + "无效的时钟源名称。"};
}

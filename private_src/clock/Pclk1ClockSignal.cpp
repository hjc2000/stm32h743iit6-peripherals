#include "Pclk1ClockSignal.h" // IWYU pragma: keep
#include "base/string/define.h"
#include <stdexcept>

void bsp::Pclk1ClockSignal::Configure(std::map<std::string, uint32_t> const &channel_factor_map)
{
	auto it = channel_factor_map.find("in");
	if (it == channel_factor_map.end())
	{
		throw std::invalid_argument{CODE_POS_STR + "channel_factor_map 中没有 in 通道。"};
	}

	RCC_ClkInitTypeDef def{};
	def.ClockType = RCC_CLOCKTYPE_PCLK1;

	switch (it->second)
	{
	case 1:
		{
			def.APB1CLKDivider = RCC_APB1_DIV1;
			break;
		}
	case 2:
		{
			def.APB1CLKDivider = RCC_APB1_DIV2;
			break;
		}
	case 4:
		{
			def.APB1CLKDivider = RCC_APB1_DIV4;
			break;
		}
	case 8:
		{
			def.APB1CLKDivider = RCC_APB1_DIV8;
			break;
		}
	case 16:
		{
			def.APB1CLKDivider = RCC_APB1_DIV16;
			break;
		}
	default:
		{
			throw std::invalid_argument{"不支持此分频"};
		}
	}

	HAL_StatusTypeDef ret = HAL_RCC_ClockConfig(&def,
												FLASH_LATENCY_2);

	if (ret != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"时钟信号配置失败"};
	}
}

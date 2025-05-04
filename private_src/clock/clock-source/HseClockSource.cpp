#include "HseClockSource.h"
#include "base/unit/MHz.h"
#include "hal.h"

base::MHz bsp::HseClockSource::Frequency() const
{
	return _frequency;
}

void bsp::HseClockSource::Configure()
{
	RCC_OscInitTypeDef def{};
	def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	def.HSEState = RCC_HSE_ON;
	HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"打开 hse 时钟源失败。"};
	}

	_frequency = base::MHz{25};
}

void bsp::HseClockSource::ConfigureAsBypassMode(base::MHz const &bypass_input_frequency)
{
	RCC_OscInitTypeDef def{};
	def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	def.HSEState = RCC_HSE_BYPASS;

	HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"设置 hse 时钟源为旁路失败。"};
	}

	_frequency = bypass_input_frequency;
}

void bsp::HseClockSource::TurnOff()
{
	RCC_OscInitTypeDef def{};
	def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	def.HSEState = RCC_HSE_OFF;

	HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"关闭 hse 时钟源失败。"};
	}

	_frequency = base::MHz{0};
}

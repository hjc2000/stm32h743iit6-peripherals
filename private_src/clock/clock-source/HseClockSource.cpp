#include "HseClockSource.h"
#include "base/exception/NotSupportedException.h"
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
	_state = IClockSource_State::On;
}

void bsp::HseClockSource::Configure(std::map<std::string, uint32_t> const &channel_factor_map)
{
	throw base::exception::NotSupportedException{};
}

void bsp::HseClockSource::SetAsBypass(base::MHz external_clock_frequency)
{
	RCC_OscInitTypeDef def{};
	def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	def.HSEState = RCC_HSE_BYPASS;

	HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"设置 hse 时钟源为旁路失败。"};
	}

	_frequency = external_clock_frequency;
	_state = IClockSource_State::Bypass;
}

void bsp::HseClockSource::Close()
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
	_state = IClockSource_State::Off;
}

bsp::IClockSource_State bsp::HseClockSource::State() const
{
	return _state;
}

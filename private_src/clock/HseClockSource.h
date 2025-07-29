#pragma once
#include "base/unit/MHz.h"
#include "clock_source_handle.h"
#include "hal.h"

namespace bsp
{
	/// @brief hse 时钟源。需要外部晶振。
	class HseClockSource :
		public base::clock::clock_source_handle
	{
	private:
		inline static base::unit::MHz _frequency{};

	public:
		virtual base::unit::MHz Frequency() const override
		{
			return _frequency;
		}

		virtual void Configure() override
		{
			RCC_OscInitTypeDef def{};
			def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
			def.HSEState = RCC_HSE_ON;
			HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
			if (result != HAL_StatusTypeDef::HAL_OK)
			{
				throw std::runtime_error{"打开 hse 时钟源失败。"};
			}

			_frequency = base::unit::MHz{25};
		}

		virtual void ConfigureAsBypassMode(base::unit::MHz const &bypass_input_frequency) override
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

		virtual void TurnOff() override
		{
			RCC_OscInitTypeDef def{};
			def.OscillatorType = RCC_OSCILLATORTYPE_HSE;
			def.HSEState = RCC_HSE_OFF;

			HAL_StatusTypeDef result = HAL_RCC_OscConfig(&def);
			if (result != HAL_StatusTypeDef::HAL_OK)
			{
				throw std::runtime_error{"关闭 hse 时钟源失败。"};
			}

			_frequency = base::unit::MHz{0};
		}
	};
} // namespace bsp

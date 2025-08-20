#pragma once
#include "base/math/pow.h"
#include "base/string/define.h"
#include "base/unit/Hz.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "independent_watch_dog_handle.h"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	class IndependentWatchDog1 final :
		public base::independent_watch_dog::independent_watch_dog_handle
	{
	private:
		base::UsageStateManager<IndependentWatchDog1> _usage_state_manager{};
		std::chrono::milliseconds _timeout{};
		IWDG_HandleTypeDef _handle{};

		///
		/// @brief 独立看门狗具有 40kHz 的内部独立时钟。
		///
		///
		base::unit::Hz _inner_clock_frequency{40 * 1000};

		///
		/// @brief 独立看门狗内部独立时钟源后方的分频器的分频系数宏定义。
		///
		///
		static constexpr uint32_t _prescaler_defines[]{
			IWDG_PRESCALER_4,
			IWDG_PRESCALER_8,
			IWDG_PRESCALER_16,
			IWDG_PRESCALER_32,
			IWDG_PRESCALER_64,
			IWDG_PRESCALER_128,
			IWDG_PRESCALER_256,
		};

		static constexpr size_t _prescaler_defines_count = sizeof(_prescaler_defines) / sizeof(_prescaler_defines[0]);

		///
		/// @brief 获取 _prescaler_defines 中对应索引的宏定义所代表的分频系数。
		///
		/// @param index
		/// @return
		///
		static constexpr uint32_t GetPrescalerFactor(size_t index)
		{
			if (index >= _prescaler_defines_count)
			{
				throw std::invalid_argument{CODE_POS_STR + "索引溢出。"};
			}

			return base::pow(static_cast<size_t>(2), index + 2);
		}

	public:
		///
		/// @brief 初始化独立看门狗。
		///
		/// @param timeout
		///
		virtual void Initialize(std::chrono::milliseconds const &timeout) override;

		///
		/// @brief 喂狗。
		///
		virtual void Feed() override
		{
			HAL_IWDG_Refresh(&_handle);
		}

		///
		/// @brief 看门狗超时时间。
		///
		/// @return
		///
		virtual std::chrono::milliseconds Timeout() override
		{
			return _timeout;
		}
	};

} // namespace bsp

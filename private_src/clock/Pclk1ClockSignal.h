#pragma once
#include "base/string/define.h"
#include "base/unit/Hz.h"
#include "clock_source_handle.h"
#include "hal.h"
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	class Pclk1ClockSignal :
		public base::clock::clock_source_handle
	{
	private:
		uint32_t _input_divider = 4;
		inline static bool _configured = false;

	public:
		virtual base::unit::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetPCLK1Freq();
			return base::unit::MHz{base::unit::Hz{value}};
		}

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override;

		///
		/// @brief 获取 PCLK1 的输入分频系数。
		///
		/// @note 这个有什么用呢？为什么要获取分频系数？
		/// 因为在 APB1 总线上的定时器的输入端有个倍频器，只要 PCLK1 的输入分频器的分频系数不是 1,
		/// 这个倍频器就会把输入定时器模块的 PCLK1 倍频。
		///
		/// @return
		///
		uint32_t InputDivider() const
		{
			if (!_configured)
			{
				throw std::runtime_error{CODE_POS_STR + "没有用本类配置过 PCLK1, 无法使用本属性。"};
			}

			return _input_divider;
		}
	};

} // namespace bsp

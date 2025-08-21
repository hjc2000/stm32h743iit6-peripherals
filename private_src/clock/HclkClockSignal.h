#pragma once
#include "base/unit/Hz.h"
#include "clock_source_handle.h"
#include "hal.h"

namespace bsp
{
	class HclkClockSignal :
		public base::clock::clock_source_handle
	{
	public:
		virtual base::unit::MHz Frequency() const override
		{
			uint32_t value = HAL_RCC_GetHCLKFreq();
			return base::unit::MHz{base::unit::Hz{value}};
		}

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override;
	};

} // namespace bsp

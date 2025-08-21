#pragma once
#include "base/unit/MHz.h"
#include "clock_source_handle.h"

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

		virtual void Configure() override;

		virtual void ConfigureAsBypassMode(base::unit::MHz const &bypass_input_frequency) override;

		virtual void TurnOff() override;
	};

} // namespace bsp

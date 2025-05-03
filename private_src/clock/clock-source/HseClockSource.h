#pragma once
#include "clock_source_handle.h"

namespace bsp
{
	/// @brief hse 时钟源。需要外部晶振。
	class HseClockSource :
		public base::clock::clock_source_handle
	{
	private:
		inline static base::MHz _frequency{};

	public:
		virtual base::MHz Frequency() const override;

		virtual void Configure() override;

		virtual void Configure(std::map<std::string, uint32_t> const &channel_factor_map) override;

		virtual void ConfigureAsBypassMode(base::MHz const &bypass_input_frequency) override;

		virtual void TurnOff() override;
	};
} // namespace bsp

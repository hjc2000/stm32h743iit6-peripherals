#pragma once
#include "clock_source_handle.h"

namespace bsp
{
	class FmcClock final :
		public base::clock::clock_source_handle
	{
	public:
		virtual base::unit::MHz Frequency() const override
		{
			throw base::NotSupportedException{};
		}

		virtual void Configure(std::string const &input_channel_name,
							   uint32_t input_divider) override
		{
			throw base::NotSupportedException{};
		}
	};

} // namespace bsp

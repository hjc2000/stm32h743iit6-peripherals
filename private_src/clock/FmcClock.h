#pragma once
#include "clock_source_handle.h"

namespace bsp
{
	class FmcClock final :
		public base::clock::clock_source_handle
	{
	private:
		enum class InputChannel
		{
			HCLK3,
			PLL1_Q,
			PLL2_R,
			PER_CK,
		};

		InputChannel _input_channel{};
		bool _configured = false;

	public:
		virtual base::unit::MHz Frequency() const override;

		virtual void Configure(std::string const &input_channel_name) override;
	};

} // namespace bsp

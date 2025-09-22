#pragma once
#include "clock_source_handle.h"

namespace bsp
{
	class FmcClock final :
		public base::clock::clock_source_handle
	{
	public:
		virtual base::unit::MHz Frequency() const
		{
			throw base::NotSupportedException{};
		}
	};

} // namespace bsp

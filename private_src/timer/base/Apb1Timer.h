#pragma once
#include "base/unit/MHz.h"

namespace bsp
{
	///
	/// @brief 位于 APB1 总线上的定时器。
	///
	///
	class Apb1Timer
	{
	public:
		static base::unit::MHz ClockSourceFrequency();
	};

} // namespace bsp

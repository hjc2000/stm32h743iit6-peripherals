#pragma once
#include "base/unit/MHz.h"
#include "base/UsageStateManager.h"

namespace bsp
{
	class Timer3
	{
	private:
		base::UsageStateManager<bsp::Timer3> _usage_state_manager{};

	public:
		static base::unit::MHz ClockSourceFrequency();
	};

} // namespace bsp

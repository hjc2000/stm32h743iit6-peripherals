#pragma once
#include "Apb1Timer.h"
#include "base/UsageStateManager.h"

namespace bsp
{
	class Timer3 :
		public bsp::Apb1Timer
	{
	private:
		base::UsageStateManager<bsp::Timer3> _usage_state_manager{};
	};

} // namespace bsp

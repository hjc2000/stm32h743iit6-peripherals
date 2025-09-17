#pragma once
#include "Apb1Timer.h"
#include "base/UsageStateManager.h"

namespace bsp
{
	class Timer5 :
		public bsp::Apb1Timer
	{
	private:
		base::UsageStateManager<bsp::Timer5> _usage_state_manager{};
	};

} // namespace bsp

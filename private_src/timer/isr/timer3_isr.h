#pragma once
#include <functional>

namespace bsp
{
	void set_timer3_isr(std::function<void()> func);

} // namespace bsp

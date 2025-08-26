#pragma once
#include <functional>

namespace bsp
{
	void set_timer5_isr(std::function<void()> func);

} // namespace bsp

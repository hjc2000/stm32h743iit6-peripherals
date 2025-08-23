#include "base_timer_handle.h" // IWYU pragma: keep
#include "base/string/define.h"
#include "BaseTimer3.h"
#include "BaseTimer6.h"
#include <stdexcept>

std::shared_ptr<base::base_timer::base_timer_handle> base::base_timer::open(uint32_t id)
{
	switch (id)
	{
	case 3:
		{
			return std::shared_ptr<base::base_timer::base_timer_handle>{new bsp::BaseTimer3{}};
		}
	case 6:
		{
			return std::shared_ptr<base::base_timer::base_timer_handle>{new bsp::BaseTimer6{}};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法 ID."};
		}
	}
}

void base::base_timer::initialize(base::base_timer::base_timer_handle &self,
								  std::chrono::nanoseconds const &period)
{
	self.Initialize(period);
}

std::chrono::nanoseconds base::base_timer::period(base::base_timer::base_timer_handle &self)
{
	return self.Period();
}

void base::base_timer::set_period(base::base_timer::base_timer_handle &self,
								  std::chrono::nanoseconds const &period)
{
	self.SetPeriod(period);
}

void base::base_timer::set_period_elapsed_callback(base::base_timer::base_timer_handle &self,
												   std::function<void()> const &callback)
{
	self.SetPeriodElapsedCallback(callback);
}

void base::base_timer::start(base::base_timer::base_timer_handle &self)
{
	self.Start();
}

void base::base_timer::stop(base::base_timer::base_timer_handle &self)
{
	self.Stop();
}

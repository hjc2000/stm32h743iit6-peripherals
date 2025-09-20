#include "check.h"
#include "base/string/define.h"
#include <stdexcept>

void bsp::sdram::check(FMC_SDRAM_TimingTypeDef &timing)
{
	if (timing.LoadToActiveDelay < 1 || timing.LoadToActiveDelay > 16)
	{
		throw std::invalid_argument{CODE_POS_STR + "非法时序。"};
	}

	if (timing.ExitSelfRefreshDelay < 1 || timing.ExitSelfRefreshDelay > 16)
	{
		throw std::invalid_argument{CODE_POS_STR + "非法时序。"};
	}

	if (timing.SelfRefreshTime < 1 || timing.SelfRefreshTime > 16)
	{
		throw std::invalid_argument{CODE_POS_STR + "非法时序。"};
	}

	if (timing.RowCycleDelay < 1 || timing.RowCycleDelay > 16)
	{
		throw std::invalid_argument{CODE_POS_STR + "非法时序。"};
	}

	if (timing.WriteRecoveryTime < 1 || timing.WriteRecoveryTime > 16)
	{
		throw std::invalid_argument{CODE_POS_STR + "非法时序。"};
	}

	if (timing.RPDelay < 1 || timing.RPDelay > 16)
	{
		throw std::invalid_argument{CODE_POS_STR + "非法时序。"};
	}

	if (timing.RCDDelay < 1 || timing.RCDDelay > 16)
	{
		throw std::invalid_argument{CODE_POS_STR + "非法时序。"};
	}
}

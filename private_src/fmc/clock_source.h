#pragma once
#include "base/unit/MHz.h"
#include <string>

namespace bsp
{
	///
	/// @brief 配置 FMC 时钟源。
	///
	/// @note https://blog.csdn.net/qq_36148047/article/details/151839506?sharetype=blogdetail&sharerId=151839506&sharerefer=PC&sharesource=qq_36148047&spm=1011.2480.3001.8118
	///
	/// @param clock_source_name
	///
	void config_fmc_clock_source(std::string const &clock_source_name);

	base::unit::MHz get_fmc_clock_source_frequency(std::string const &clock_source_name);

} // namespace bsp

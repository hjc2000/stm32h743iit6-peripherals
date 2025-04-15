#include "sdram_controller_handle.h"
#include <bsp-interface/di/sdram.h>

bsp::sdram::ISDRAMController &bsp::di::sdram::SDRAMController()
{
	return base::sdram::sdram_controller_handle::Instance();
}

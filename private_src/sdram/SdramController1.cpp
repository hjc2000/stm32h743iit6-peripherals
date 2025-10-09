#include "SdramController1.h"
#include "base/embedded/clock/ClockSource.h"
#include "check.h"
#include "define.h"

void bsp::SdramController1::InitializeAsReadBurstMode(std::string const &clock_source_name,
													  uint32_t divider,
													  base::sdram::ISDRAMTimingProvider const &timing_provider,
													  base::sdram::BankCount const &bank_count,
													  base::sdram::RowBitCount const &row_bit_count,
													  base::sdram::ColumnBitCount const &column_bit_count,
													  base::sdram::DataWidth const &data_width,
													  base::sdram::ReadBurstLength const &read_burst_length)
{
	__HAL_RCC_FMC_CLK_ENABLE();

	base::clock::ClockSource fmc{"fmc"};
	fmc.Configure(clock_source_name);
	base::unit::MHz clock_source_freq = fmc.Frequency();
	_timing = timing_provider.GetTiming(base::unit::MHz{clock_source_freq / divider});

	_handle.Instance = FMC_SDRAM_DEVICE;
	_handle.Init.SDBank = FMC_SDRAM_BANK1;
	_handle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	_handle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
	_handle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;
	_handle.Init.InternalBankNumber = bsp::sdram::bank_count_to_define(bank_count);
	_handle.Init.RowBitsNumber = bsp::sdram::row_bit_count_to_define(row_bit_count);
	_handle.Init.ColumnBitsNumber = bsp::sdram::column_bit_count_to_define(column_bit_count);
	_handle.Init.MemoryDataWidth = bsp::sdram::data_width_to_define(data_width);
	_handle.Init.SDClockPeriod = bsp::sdram::clock_source_divider_value_to_define(divider);
	_handle.Init.CASLatency = bsp::sdram::cas_latency_value_to_define(_timing.cas_latency());

	FMC_SDRAM_TimingTypeDef timing_def{};
	timing_def.LoadToActiveDelay = _timing.t_rsc_clock_cycle_count();
	timing_def.ExitSelfRefreshDelay = _timing.t_xsr_clock_cycle_count();
	timing_def.SelfRefreshTime = _timing.t_ras_clock_cycle_count();
	timing_def.RowCycleDelay = _timing.t_rc_clock_cycle_count();
	timing_def.WriteRecoveryTime = _timing.t_wr_clock_cycle_count();
	timing_def.RPDelay = _timing.t_rp_clock_cycle_count();
	timing_def.RCDDelay = _timing.t_rcd_clock_cycle_count();
	bsp::sdram::check(timing_def);

	HAL_SDRAM_Init(&_handle, &timing_def);
	PowerUp();

	{
		// 启动 SDRAM 控制器，开始定时发送自动刷新命令。
		int refresh_count = _timing.auto_refresh_command_clock_cycle_count() - 50;
		if (refresh_count < 50)
		{
			throw std::runtime_error{"FMC 的频率过低导致几乎一直都要处于发送自动刷新命令的状态。"};
		}

		HAL_SDRAM_ProgramRefreshRate(&_handle, refresh_count);
	}
}

void bsp::SdramController1::PowerUp()
{
	FMC_SDRAM_CommandTypeDef command{};
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = 0;
	HAL_StatusTypeDef result = HAL_SDRAM_SendCommand(&_handle, &command, 0XFFFF);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"SDRAM 上电失败。"};
	}
}

void bsp::SdramController1::PrechargeAll()
{
	FMC_SDRAM_CommandTypeDef command{};
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.CommandMode = FMC_SDRAM_CMD_PALL;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = 0;
	HAL_StatusTypeDef result = HAL_SDRAM_SendCommand(&_handle, &command, 0XFFFF);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"发送 “预充电所有 BANK” 命令失败。"};
	}
}

void bsp::SdramController1::AutoRefresh()
{
	FMC_SDRAM_CommandTypeDef command{};
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = 0;
	HAL_StatusTypeDef result = HAL_SDRAM_SendCommand(&_handle, &command, 0XFFFF);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"发送 “自动刷新” 命令失败。"};
	}
}

void bsp::SdramController1::WriteModeRegister(uint32_t value)
{
	FMC_SDRAM_CommandTypeDef command{};
	command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	command.AutoRefreshNumber = 1;
	command.ModeRegisterDefinition = value;
	HAL_StatusTypeDef result = HAL_SDRAM_SendCommand(&_handle, &command, 0XFFFF);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"发送 “写模式寄存器” 命令失败。"};
	}
}

base::sdram::sdram_timing const &bsp::SdramController1::Timing() const
{
	return _timing;
}

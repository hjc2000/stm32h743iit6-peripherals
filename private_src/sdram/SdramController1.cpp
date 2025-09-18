#include "SdramController1.h"
#include "base/embedded/clock/ClockSource.h"
#include "define.h"

void bsp::SdramController1::InitializeAsReadBurstMode(base::sdram::ISDRAMTimingProvider const &timing_provider,
													  base::sdram::BankCount const &bank_count,
													  base::sdram::RowBitCount const &row_bit_count,
													  base::sdram::ColumnBitCount const &column_bit_count,
													  base::sdram::DataWidth const &data_width,
													  base::sdram::ReadBurstLength const &read_burst_length)
{
	/* 保护SDRAM区域,共32M字节 */
	mpu_set_protection(0xC0000000,               /* 基地址 */
					   MPU_REGION_SIZE_32MB,     /* 长度 */
					   MPU_REGION_NUMBER6, 0,    /* NUMER6,允许指令访问 */
					   MPU_REGION_FULL_ACCESS,   /* 全访问 */
					   MPU_ACCESS_NOT_SHAREABLE, /* 禁止共用 */
					   MPU_ACCESS_CACHEABLE,     /* 允许cache */
					   MPU_ACCESS_BUFFERABLE);   /* 允许缓冲 */

	__HAL_RCC_FMC_CLK_ENABLE();

	_handle.Instance = FMC_SDRAM_DEVICE;
	_handle.Init.SDBank = FMC_SDRAM_BANK1;
	_handle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	_handle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
	_handle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;
	_handle.Init.InternalBankNumber = bsp::sdram::bank_count_to_define(bank_count);
	_handle.Init.RowBitsNumber = bsp::sdram::row_bit_count_to_define(row_bit_count);
	_handle.Init.ColumnBitsNumber = bsp::sdram::column_bit_count_to_define(column_bit_count);
	_handle.Init.MemoryDataWidth = bsp::sdram::data_width_to_define(data_width);

	// 初始化 _timing
	{
		base::clock::ClockSource hclk{"hclk"};
		base::unit::MHz hclk_freq = hclk.Frequency();

		// 分频系数
		int hclk_div = 2;
		_handle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;

		if (hclk_freq / hclk_div > timing_provider.MaxClockFrequency())
		{
			hclk_div = 3;
			_handle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_3;
		}

		_timing = timing_provider.GetTiming(base::unit::MHz{hclk_freq / hclk_div});
	}

	_handle.Init.CASLatency = bsp::sdram::cas_latency_value_to_define(_timing.cas_latency());

	FMC_SDRAM_TimingTypeDef timing_def{};
	timing_def.LoadToActiveDelay = _timing.t_rsc_clock_cycle_count();
	timing_def.ExitSelfRefreshDelay = _timing.t_xsr_clock_cycle_count();
	timing_def.SelfRefreshTime = _timing.t_ras_clock_cycle_count();
	timing_def.RowCycleDelay = _timing.t_rc_clock_cycle_count();
	timing_def.WriteRecoveryTime = _timing.t_wr_clock_cycle_count();
	timing_def.RPDelay = _timing.t_rp_clock_cycle_count();
	timing_def.RCDDelay = _timing.t_rcd_clock_cycle_count();

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

#include "sdram_controller_handle.h"
#include "bsp-interface/di/clock.h"

void base::sdram::sdram_controller_handle::InitializeGPIO()
{
	for (auto &pin : _pins)
	{
		pin.InitializeAsAlternateFunctionMode(base::gpio::AlternateFunction::SDRAM1,
											  base::gpio::PullMode::PullUp,
											  base::gpio::DriveMode::PushPull);
	}
}

void base::sdram::sdram_controller_handle::StartAutoSendingAutoRefreshCommand(base::sdram::sdram_timing const &timing)
{
	int refresh_count = timing.auto_refresh_command_clock_count() - 50;
	if (refresh_count < 50)
	{
		throw std::runtime_error{"FMC 的频率过低导致几乎一直都要处于发送自动刷新命令的状态。"};
	}

	HAL_SDRAM_ProgramRefreshRate(&_handle, refresh_count);
}

void base::sdram::sdram_controller_handle::OpenAsReadBurstMode(base::sdram::ISDRAMTimingProvider const &timing_provider,
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
	InitializeGPIO();

	_handle.Instance = FMC_SDRAM_DEVICE;
	_handle.Init.SDBank = FMC_SDRAM_BANK1;
	_handle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	_handle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
	_handle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_2;

	switch (row_bit_count.Value())
	{
	case 11:
		{
			_handle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_11;
			break;
		}
	case 12:
		{
			_handle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
			break;
		}
	case 13:
		{
			_handle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
			break;
		}
	default:
		{
			throw std::invalid_argument{"不支持的行地址位数。"};
		}
	}

	switch (column_bit_count.Value())
	{
	case 8:
		{
			_handle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
			break;
		}
	case 9:
		{
			_handle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
			break;
		}
	case 10:
		{
			_handle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_10;
			break;
		}
	case 11:
		{
			_handle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_11;
			break;
		}
	default:
		{
			throw std::invalid_argument{"不支持的列地址位数。"};
		}
	}

	switch (data_width.Value())
	{
	case 8:
		{
			_handle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_8;
			break;
		}
	case 16:
		{
			_handle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
			break;
		}
	case 32:
		{
			_handle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
			break;
		}
	default:
		{
			throw std::invalid_argument{"不支持的数据宽度。"};
		}
	}

	switch (bank_count.Value())
	{
	case 2:
		{
			_handle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_2;
			break;
		}
	case 4:
		{
			_handle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
			break;
		}
	default:
		{
			throw std::invalid_argument{"不支持的 BANK 数量。"};
		}
	}

	// 初始化 _timing
	{
		base::MHz hclk_freq = bsp::di::clock::ClockSignalCollection().Get("hclk")->Frequency();

		// 分频系数
		int hclk_div = 2;
		_handle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;

		if (hclk_freq / hclk_div > timing_provider.MaxClockFrequency())
		{
			hclk_div = 3;
			_handle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_3;
		}

		_timing = timing_provider.GetTiming(base::MHz{hclk_freq / hclk_div});
	}

	switch (_timing.cas_latency())
	{
	case 1:
		{
			_handle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_1;
			break;
		}
	case 2:
		{
			_handle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
			break;
		}
	case 3:
		{
			_handle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
			break;
		}
	default:
		{
			throw std::invalid_argument{"不支持的 CASLatency."};
		}
	}

	FMC_SDRAM_TimingTypeDef timing_def{};
	timing_def.LoadToActiveDelay = _timing.t_rsc_clock_count();
	timing_def.ExitSelfRefreshDelay = _timing.t_xsr_clock_count();
	timing_def.SelfRefreshTime = _timing.t_ras_clock_count();
	timing_def.RowCycleDelay = _timing.t_rc_clock_count();
	timing_def.WriteRecoveryTime = _timing.t_wr_clock_count();
	timing_def.RPDelay = _timing.t_rp_clock_count();
	timing_def.RCDDelay = _timing.t_rcd_clock_count();

	HAL_SDRAM_Init(&_handle, &timing_def);
	PowerUp();
	StartAutoSendingAutoRefreshCommand(_timing);
}

void base::sdram::sdram_controller_handle::PowerUp()
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

void base::sdram::sdram_controller_handle::PrechargeAll()
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

void base::sdram::sdram_controller_handle::AutoRefresh()
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

void base::sdram::sdram_controller_handle::WriteModeRegister(uint32_t value)
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

base::sdram::sdram_timing const &base::sdram::sdram_controller_handle::Timing() const
{
	return _timing;
}

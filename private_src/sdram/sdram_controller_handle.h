#pragma once
#include "base/embedded/sdram/sdram_controller_handle.h"

class base::sdram::sdram_controller_handle
{
public:
	virtual ~sdram_controller_handle() = default;

	///
	/// @brief 将 SDRAM 控制器以读突发的模式打开。写不突发。
	///
	/// @param clock_source_name
	/// @param divider
	/// @param timing_provider
	/// @param bank_count
	/// @param row_bit_count
	/// @param column_bit_count
	/// @param data_width
	/// @param read_burst_length
	///
	virtual void InitializeAsReadBurstMode(std::string const &clock_source_name,
										   uint32_t divider,
										   base::sdram::ISDRAMTimingProvider const &timing_provider,
										   base::sdram::BankCount const &bank_count,
										   base::sdram::RowBitCount const &row_bit_count,
										   base::sdram::ColumnBitCount const &column_bit_count,
										   base::sdram::DataWidth const &data_width,
										   base::sdram::ReadBurstLength const &read_burst_length) = 0;

	///
	/// @brief 将输入信号置于空操作命令状态，然后开始向 SDRAM 提供 CLK 信号。
	///
	virtual void PowerUp() = 0;

	///
	/// @brief 发送：“预充电所有 BANK” 的命令。
	///
	virtual void PrechargeAll() = 0;

	///
	/// @brief 发送自动刷新命令。
	///
	virtual void AutoRefresh() = 0;

	///
	/// @brief 写模式寄存器。
	/// @param value
	///
	virtual void WriteModeRegister(uint32_t value) = 0;

	///
	/// @brief 控制器被打开后所使用的时序。
	/// @return
	///
	virtual base::sdram::sdram_timing const &Timing() const = 0;

	///
	/// @brief 此 SDRAM 控制器所管理的内存段的起始地址。打开 SDRAM 后，对着这个地址开始往后的内存区域
	/// 读写数据即可读写 SDRAM 的内容。
	///
	/// @return
	///
	virtual uint8_t *StartAddress() const = 0;
};

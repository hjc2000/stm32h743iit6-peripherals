#pragma once
#include "base/embedded/sdram/sdram_timing.h"
#include "base/UsageStateManager.h"
#include "sdram_controller_handle.h"
#include <base/define.h>
#include <hal.h>

namespace bsp
{
	/// @brief 封装 FMC 接口。
	class SdramController1 final :
		public base::sdram::sdram_controller_handle
	{
	private:
		base::UsageStateManager<SdramController1> _usage_manager{};
		SDRAM_HandleTypeDef _handle{};

		base::sdram::sdram_timing _timing{};

	public:
		SdramController1()
		{
			base::sdram::msp_initialize_callback(1);
		}

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
											   base::sdram::ReadBurstLength const &read_burst_length) override;

		///
		/// @brief 将输入信号置于空操作命令状态，然后开始向 SDRAM 提供 CLK 信号。
		///
		void PowerUp() override;

		///
		/// @brief 发送：“预充电所有 BANK” 的命令。
		///
		void PrechargeAll() override;

		///
		/// @brief 发送自动刷新命令。
		///
		void AutoRefresh() override;

		///
		/// @brief 写模式寄存器。
		///
		/// @param value
		///
		void WriteModeRegister(uint32_t value) override;

		///
		/// @brief 控制器被打开后所使用的时序。
		///
		/// @return
		///
		base::sdram::sdram_timing const &Timing() const override;

		///
		/// @brief 此 SDRAM 控制器所管理的内存段的起始地址。打开 SDRAM 后，对着这个地址开始往后的内存区域
		/// 读写数据即可读写 SDRAM 的内容。
		///
		/// @return
		///
		uint8_t *StartAddress() const override
		{
			return reinterpret_cast<uint8_t *>(0xC0000000);
		}
	};

} // namespace bsp

#include "base/stream/Span.h"
#include "sdram_controller_handle.h"

std::shared_ptr<base::sdram::sdram_controller_handle> base::sdram::open()
{
	return std::shared_ptr<base::sdram::sdram_controller_handle>{new base::sdram::sdram_controller_handle{}};
}

void base::sdram::initialize_as_read_burst_mode(base::sdram::sdram_controller_handle &h,
												base::sdram::ISDRAMTimingProvider const &timing_provider,
												base::sdram::BankCount const &bank_count,
												base::sdram::RowBitCount const &row_bit_count,
												base::sdram::ColumnBitCount const &column_bit_count,
												base::sdram::DataWidth const &data_width,
												base::sdram::ReadBurstLength const &read_burst_length)
{
	h.OpenAsReadBurstMode(timing_provider,
						  bank_count,
						  row_bit_count,
						  column_bit_count,
						  data_width,
						  read_burst_length);
}

void base::sdram::send_precharge_all_command(base::sdram::sdram_controller_handle &h)
{
	h.PrechargeAll();
}

void base::sdram::send_auto_refresh_command(base::sdram::sdram_controller_handle &h)
{
	h.AutoRefresh();
}

void base::sdram::send_auto_refresh_command(base::sdram::sdram_controller_handle &h, int refresh_times)
{
	for (int i = 0; i < refresh_times; i++)
	{
		h.AutoRefresh();
	}
}

void base::sdram::write_mode_register(base::sdram::sdram_controller_handle &h, uint32_t value)
{
	h.WriteModeRegister(value);
}

base::sdram::sdram_timing const &base::sdram::timing(base::sdram::sdram_controller_handle &h)
{
	return h.Timing();
}

base::Span base::sdram::span(base::sdram::sdram_controller_handle &h)
{
	return base::Span{h.StartAddress(), 32 * 1024 * 1024};
}

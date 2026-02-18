#include "sdram_controller_handle.h"
#include "base/stream/Span.h"
#include "SdramController1.h"

std::shared_ptr<base::sdram::sdram_controller_handle> base::sdram::open(uint32_t id)
{
	return std::shared_ptr<base::sdram::sdram_controller_handle>{new bsp::SdramController1{}};
}

void base::sdram::initialize_as_read_burst_mode(base::sdram::sdram_controller_handle &h,
												std::string const &clock_source_name,
												uint32_t divider,
												base::sdram::ISDRAMTimingProvider const &timing_provider,
												base::sdram::BankCount const &bank_count,
												base::sdram::RowBitCount const &row_bit_count,
												base::sdram::ColumnBitCount const &column_bit_count,
												base::sdram::DataWidth const &data_width,
												base::sdram::ReadBurstLength const &read_burst_length)
{
	h.InitializeAsReadBurstMode(clock_source_name,
								divider,
								timing_provider,
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
	return h.Span();
}

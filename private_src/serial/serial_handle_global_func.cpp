#include "base/embedded/serial/serial_handle.h"
#include "Serial1.h"

std::shared_ptr<base::serial::serial_handle> base::serial::open(uint32_t id)
{
	return std::shared_ptr<base::serial::serial_handle>{new bsp::Serial1{}};
}

void base::serial::start(base::serial::serial_handle &h,
						 base::serial::Direction direction,
						 base::serial::BaudRate const &baud_rate,
						 base::serial::DataBits const &data_bits,
						 base::serial::Parity parity,
						 base::serial::StopBits stop_bits,
						 base::serial::HardwareFlowControl hardware_flow_control)
{
	h.Start(direction,
			baud_rate,
			data_bits,
			parity,
			stop_bits,
			hardware_flow_control);
}

/* #region 串口属性 */

base::serial::Direction base::serial::direction(base::serial::serial_handle &h)
{
	return h.Direction();
}

uint32_t base::serial::baud_rate(base::serial::serial_handle &h)
{
	return h.BaudRate();
}

uint8_t base::serial::data_bits(base::serial::serial_handle &h)
{
	return h.DataBits();
}

base::serial::Parity base::serial::parity(base::serial::serial_handle &h)
{
	return h.Parity();
}

base::serial::StopBits base::serial::stop_bits(base::serial::serial_handle &h)
{
	return h.StopBits();
}

base::serial::HardwareFlowControl base::serial::hardware_flow_control(base::serial::serial_handle &h)
{
	return h.HardwareFlowControl();
}

bool base::serial::can_read(base::serial::serial_handle &h)
{
	return h.CanRead();
}

bool base::serial::can_write(base::serial::serial_handle &h)
{
	return h.CanWrite();
}

/* #endregion */

int32_t base::serial::read(base::serial::serial_handle &h, base::Span const &span)
{
	return h.Read(span);
}

void base::serial::write(base::serial::serial_handle &h, base::ReadOnlySpan const &span)
{
	h.Write(span);
}

void base::serial::flush(base::serial::serial_handle &h)
{
}

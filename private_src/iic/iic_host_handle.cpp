#include "iic_host_handle.h"
#include "SoftwareIicHost.h"

std::shared_ptr<base::iic::iic_host_handle> base::iic::open(std::shared_ptr<base::iic::ISoftwareIicHostPinDriver> const &pin_driver)
{
	return std::shared_ptr<base::iic::iic_host_handle>{new bsp::SoftwareIicHost{pin_driver}};
}

void base::iic::initialize(base::iic::iic_host_handle &h,
						   base::unit::Nanoseconds const &scl_cycle,
						   base::unit::Nanoseconds const &waiting_for_ack_signal_timeout)
{
	h.Initialize(scl_cycle,
				 waiting_for_ack_signal_timeout);
}

void base::iic::send_starting_signal(base::iic::iic_host_handle &h)
{
	h.SendStartingSignal();
}

void base::iic::send_stopping_signal(base::iic::iic_host_handle &h)
{
	h.SendStoppingSignal();
}

void base::iic::send_byte(base::iic::iic_host_handle &h, uint8_t byte)
{
	h.SendByte(byte);
}

uint8_t base::iic::receive_byte(base::iic::iic_host_handle &h, bool send_nack)
{
	return h.ReceiveByte(send_nack);
}

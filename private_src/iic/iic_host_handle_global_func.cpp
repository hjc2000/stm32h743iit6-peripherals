#include "iic_host_handle.h"

std::shared_ptr<base::iic::iic_host_handle> base::iic::open(std::shared_ptr<base::iic::ISoftwareIicHostPinDriver> const &pin_driver);

///
/// @brief 初始化 IIC 主机接口。
///
/// @param h
/// @param scl_cycle
/// @param waiting_for_ack_signal_timeout
///
void base::iic::initialize(base::iic::iic_host_handle &h,
						   base::Nanoseconds const &scl_cycle,
						   base::Nanoseconds const &waiting_for_ack_signal_timeout);

///
/// @brief 发送 IIC 启动信号。
///
/// @param h
///
void base::iic::send_starting_signal(base::iic::iic_host_handle &h);

///
/// @brief 发送 IIC 停止信号。
///
/// @param h
///
void base::iic::send_stopping_signal(base::iic::iic_host_handle &h);

///
/// @brief 发送一个字节。
///
/// @param h
/// @param byte
///
void base::iic::send_byte(base::iic::iic_host_handle &h, uint8_t byte);

///
/// @brief 读取 1 个字节。
///
/// @param h
///
/// @param send_nack 读取完这个字节后是否发送 nack 信号终止读取。
/// 	@li 传入 true 则发送 nack 信号表示不想继续读了。
/// 	@li 传入 false 则发送 ack 信号，表示希望继续读取。
///
/// @return
///
uint8_t base::iic::receive_byte(base::iic::iic_host_handle &h, bool send_nack);

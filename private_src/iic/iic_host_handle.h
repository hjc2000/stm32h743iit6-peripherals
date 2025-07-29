#pragma once
#include "base/embedded/iic/iic_host_handle.h"

class base::iic::iic_host_handle
{
public:
	///
	/// @brief 初始化 IIC 主机接口。
	///
	/// @param scl_cycle
	/// @param waiting_for_ack_signal_timeout
	///
	virtual void Initialize(base::unit::Nanoseconds const &scl_cycle,
							base::unit::Nanoseconds const &waiting_for_ack_signal_timeout) = 0;

	///
	/// @brief 发送 IIC 启动信号。
	///
	///
	virtual void SendStartingSignal() = 0;

	///
	/// @brief 发送 IIC 停止信号。
	///
	///
	virtual void SendStoppingSignal() = 0;

	///
	/// @brief 发送一个字节。
	///
	/// @param byte
	///
	virtual void SendByte(uint8_t byte) = 0;

	///
	/// @brief 读取 1 个字节。
	///
	/// @param send_nack 读取完这个字节后是否发送 nack 信号终止读取。
	/// 	@li 传入 true 则发送 nack 信号表示不想继续读了。
	/// 	@li 传入 false 则发送 ack 信号，表示希望继续读取。
	///
	/// @return
	///
	virtual uint8_t ReceiveByte(bool send_nack) = 0;
};

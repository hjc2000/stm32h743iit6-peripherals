#pragma once
#include "base/embedded/iic/SoftwareIicHost.h"
#include "iic_host_handle.h"

namespace bsp
{
	class SoftwareIicHost1 :
		public base::iic::iic_host_handle
	{
	private:
		base::iic::SoftwareIicHost _iic_host;

	public:
		SoftwareIicHost1(std::shared_ptr<base::iic::ISoftwareIicHostPinDriver> const &pin_driver)
			: _iic_host(pin_driver)
		{
		}

		///
		/// @brief 初始化 IIC 主机接口。
		///
		/// @param scl_cycle
		/// @param waiting_for_ack_signal_timeout
		///
		virtual void Initialize(base::Nanoseconds const &scl_cycle,
								base::Nanoseconds const &waiting_for_ack_signal_timeout) override
		{
			_iic_host.Initialize(scl_cycle,
								 waiting_for_ack_signal_timeout);
		}

		///
		/// @brief 发送 IIC 启动信号。
		///
		///
		virtual void SendStartingSignal() override
		{
			_iic_host.SendStartingSignal();
		}

		///
		/// @brief 发送 IIC 停止信号。
		///
		///
		virtual void SendStoppingSignal() override
		{
			_iic_host.SendStoppingSignal();
		}

		///
		/// @brief 发送一个字节。
		///
		/// @param byte
		///
		virtual void SendByte(uint8_t byte) override
		{
			_iic_host.SendByte(byte);
		}

		///
		/// @brief 读取 1 个字节。
		///
		/// @param send_nack 读取完这个字节后是否发送 nack 信号终止读取。
		/// 	@li 传入 true 则发送 nack 信号表示不想继续读了。
		/// 	@li 传入 false 则发送 ack 信号，表示希望继续读取。
		///
		/// @return
		///
		virtual uint8_t ReceiveByte(bool send_nack) override
		{
			return _iic_host.ReceiveByte(send_nack);
		}
	};
} // namespace bsp

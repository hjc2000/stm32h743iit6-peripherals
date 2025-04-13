#pragma once
#include "base/peripheral/serial/serial_handle.h"

class base::serial::serial_handle
{
public:
	///
	/// @brief 启动串口。
	///
	/// @param direction 串口数据方向。可以选择只发、只收、收发。
	/// @param baud_rate 波特率。
	/// @param data_bits 数据位位数。
	/// @param parity 奇偶校验。
	/// @param stop_bits 停止位位数。
	/// @param hardware_flow_control 硬件流控。
	///
	virtual void Start(base::serial::Direction direction,
					   base::serial::BaudRate const &baud_rate,
					   base::serial::DataBits const &data_bits,
					   base::serial::Parity parity,
					   base::serial::StopBits stop_bits,
					   base::serial::HardwareFlowControl hardware_flow_control) = 0;

	/* #region 串口属性 */

	/// @brief 数据传输方向
	/// @return
	virtual base::serial::Direction Direction() const = 0;

	/// @brief 波特率。
	/// @return
	virtual uint32_t BaudRate() const = 0;

	/// @brief 数据位的个数。
	/// @return
	virtual uint8_t DataBits() const = 0;

	/// @brief 校验位。
	/// @return
	virtual base::serial::Parity Parity() const = 0;

	/// @brief 停止位个数。
	/// @return
	virtual base::serial::StopBits StopBits() const = 0;

	/// @brief 硬件流控。
	/// @return
	virtual base::serial::HardwareFlowControl HardwareFlowControl() const = 0;

	///
	/// @brief 本流能否读取。
	///
	/// @return true 能读取。
	/// @return false 不能读取。
	///
	bool CanRead() const
	{
		if (Direction() == base::serial::Direction::RX)
		{
			return true;
		}

		if (Direction() == base::serial::Direction::RX_TX)
		{
			return true;
		}

		return false;
	}

	///
	/// @brief 本流能否写入。
	///
	/// @return true 能写入。
	/// @return false 不能写入。
	///
	bool CanWrite() const
	{
		if (Direction() == base::serial::Direction::TX)
		{
			return true;
		}

		if (Direction() == base::serial::Direction::RX_TX)
		{
			return true;
		}

		return false;
	}

	/* #endregion */

	///
	/// @brief 调用后临时启动 DMA 接收一次数据。
	///
	/// @note 本类没有缓冲机制，所以上层应用如果调用 Read 不及时，会丢失数据。
	///
	/// @note 因为调用一次 Read 具有一定开销，需要设置寄存器，使能中断，设置一些
	/// 状态变量。所以为了提高效率，每次调用 Read 时传入的 buffer 适当大一些，
	/// 并且 count 大一些。
	///
	/// @param span
	///
	/// @return
	///
	virtual int32_t Read(base::Span const &span) = 0;

	///
	/// @brief 调用后临时启动 DMA 进行一次发送。
	/// @param span
	///
	virtual void Write(base::ReadOnlySpan const &span) = 0;

	///
	/// @brief 冲洗流。
	///
	/// @note 对于写入的数据，作用是将其从内部缓冲区转移到底层。
	/// @note 对于内部的可以读取但尚未读取的数据，一般不会有什么作用。Flush 没见过对可读数据生效的。
	///
	virtual void Flush() = 0;
};

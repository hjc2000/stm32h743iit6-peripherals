#pragma once
#include "base/define.h"
#include "base/peripheral/gpio/gpio_parameter.h"
#include "base/peripheral/gpio/GpioPin.h"
#include "base/peripheral/serial/serial_handle.h"
#include "base/string/define.h"
#include "base/task/IBinarySemaphore.h"
#include "base/task/IMutex.h"
#include "hal.h"
#include <stdexcept>

class base::serial::serial_handle
{
private:
	serial_handle() = default;

	UART_HandleTypeDef _uart_handle{};
	std::shared_ptr<base::IBinarySemaphore> _sending_completion_signal = base::CreateIBinarySemaphore(false);
	std::shared_ptr<base::IBinarySemaphore> _receiving_completion_signal = base::CreateIBinarySemaphore(false);
	std::shared_ptr<base::IMutex> _read_lock = base::CreateIMutex();
	DMA_HandleTypeDef _rx_dma_handle{};
	DMA_HandleTypeDef _tx_dma_handle{};
	base::gpio::GpioPin _pa9{base::gpio::PortEnum::PortA, 9};
	base::gpio::GpioPin _pa10{base::gpio::PortEnum::PortA, 10};
	base::serial::Direction _direction;
	uint32_t _baud_rate;
	uint8_t _data_bits;
	base::serial::Parity _parity;
	base::serial::StopBits _stop_bits;
	base::serial::HardwareFlowControl _hardware_flow_control;

	/* #region 初始化 */
	void InitializeGpio();
	void InitializeRxDma();
	void InitializeTxDma();
	void InitializeUart();
	void InitializeInterrupt();
	/* #endregion */

	/// @brief 通过串口句柄和 DMA 寄存器，获取当前 DMA 接收了多少个字节。
	/// @return
	int32_t HaveRead();

	/* #region 被中断处理函数回调的函数 */
	static_function void OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos);
	static_function void OnSendCompleteCallback(UART_HandleTypeDef *huart);
	static_function void OnReadTimeout(UART_HandleTypeDef *huart);
	/* #endregion */

	void SetReadTimeoutByBaudCount(uint32_t value);

public:
	static_function serial_handle &Instance();

	///
	/// @brief 打开串口。
	///
	/// @param direction 串口数据方向。可以选择只发、只收、收发。
	/// @param baud_rate 波特率。
	/// @param data_bits 数据位位数。
	/// @param parity 奇偶校验。
	/// @param stop_bits 停止位位数。
	/// @param hardware_flow_control 硬件流控。
	///
	void Start(base::serial::Direction direction,
			   base::serial::BaudRate const &baud_rate,
			   base::serial::DataBits const &data_bits,
			   base::serial::Parity parity,
			   base::serial::StopBits stop_bits,
			   base::serial::HardwareFlowControl hardware_flow_control);

	/* #region 串口属性 */

	/// @brief 数据传输方向
	/// @return
	base::serial::Direction Direction() const
	{
		return _direction;
	}

	/// @brief 波特率。
	/// @return
	uint32_t BaudRate() const
	{
		return _baud_rate;
	}

	/// @brief 数据位的个数。
	/// @return
	uint8_t DataBits() const
	{
		return _data_bits;
	}

	/// @brief 校验位。
	/// @return
	base::serial::Parity Parity() const
	{
		return _parity;
	}

	/// @brief 停止位个数。
	/// @return
	base::serial::StopBits StopBits() const
	{
		return _stop_bits;
	}

	/// @brief 硬件流控。
	/// @return
	base::serial::HardwareFlowControl HardwareFlowControl() const
	{
		return _hardware_flow_control;
	}

	/* #endregion */

	/* #region 流属性 */

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

	///
	/// @brief 本流能否定位。
	///
	/// @return true 能定位。
	/// @return false 不能定位。
	///
	bool CanSeek() const
	{
		return false;
	}

	///
	/// @brief 流的长度
	///
	/// @return int64_t
	///
	int64_t Length() const
	{
		throw std::runtime_error{CODE_POS_STR + "不支持的功能。"};
	}

	///
	/// @brief 设置流的长度。
	///
	/// @param value
	///
	void SetLength(int64_t value)
	{
		throw std::runtime_error{CODE_POS_STR + "不支持的功能。"};
	}

	///
	/// @brief 流当前的位置。
	///
	/// @return int64_t
	///
	int64_t Position() const
	{
		throw std::runtime_error{CODE_POS_STR + "不支持的功能。"};
	}

	///
	/// @brief 设置流当前的位置。
	///
	/// @param value
	///
	void SetPosition(int64_t value)
	{
		throw std::runtime_error{CODE_POS_STR + "不支持的功能。"};
	}

	/* #endregion */

	/* #region 读写冲关 */

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
	int32_t Read(base::Span const &span);

	///
	/// @brief 调用后临时启动 DMA 进行一次发送。
	/// @param span
	///
	void Write(base::ReadOnlySpan const &span);

	///
	/// @brief 冲洗流。
	///
	/// @note 对于写入的数据，作用是将其从内部缓冲区转移到底层。
	/// @note 对于内部的可以读取但尚未读取的数据，一般不会有什么作用。Flush 没见过对可读数据生效的。
	///
	void Flush()
	{
	}

	///
	/// @brief 关闭流。
	///
	/// @note 关闭后对流的操作将会引发异常。
	///
	/// @note 因为本接口是串口接口，所以关闭流也等于关闭串口。
	///
	void Close();

	/* #endregion */
};

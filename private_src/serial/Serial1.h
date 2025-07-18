#pragma once
#include "base/define.h"
#include "base/embedded/gpio/gpio_parameter.h"
#include "base/embedded/gpio/GpioPin.h"
#include "base/task/BinarySemaphore.h"
#include "base/task/Mutex.h"
#include "base/UsageStateManager.h"
#include "hal.h"
#include "serial_handle.h"
#include <atomic>

namespace bsp
{
	class Serial1 :
		public base::serial::serial_handle
	{
	private:
		base::UsageStateManager<Serial1> _usage_state_maneger{};

		class handle_context
		{
		public:
			handle_context(Serial1 *self)
			{
				_self = self;
			}

			UART_HandleTypeDef _uart_handle{};
			Serial1 *_self{};
		};

		handle_context _handle_context{this};
		base::task::BinarySemaphore _sending_completion_signal{false};
		base::task::BinarySemaphore _receiving_completion_signal{false};
		base::task::Mutex _read_lock{};
		base::task::Mutex _write_lock{};
		DMA_HandleTypeDef _rx_dma_handle{};
		DMA_HandleTypeDef _tx_dma_handle{};
		base::gpio::GpioPin _pa9{base::gpio::PortEnum::PortA, 9};
		base::gpio::GpioPin _pa10{base::gpio::PortEnum::PortA, 10};
		std::atomic_bool _closed = false;

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
		~Serial1();

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
		virtual void Start(base::serial::Direction direction,
						   base::serial::BaudRate const &baud_rate,
						   base::serial::DataBits const &data_bits,
						   base::serial::Parity parity,
						   base::serial::StopBits stop_bits,
						   base::serial::HardwareFlowControl hardware_flow_control) override;

		/* #region 串口属性 */

		/// @brief 数据传输方向
		/// @return
		virtual base::serial::Direction Direction() const override
		{
			return _direction;
		}

		/// @brief 波特率。
		/// @return
		virtual uint32_t BaudRate() const override
		{
			return _baud_rate;
		}

		/// @brief 数据位的个数。
		/// @return
		virtual uint8_t DataBits() const override
		{
			return _data_bits;
		}

		/// @brief 校验位。
		/// @return
		virtual base::serial::Parity Parity() const override
		{
			return _parity;
		}

		/// @brief 停止位个数。
		/// @return
		virtual base::serial::StopBits StopBits() const override
		{
			return _stop_bits;
		}

		/// @brief 硬件流控。
		/// @return
		virtual base::serial::HardwareFlowControl HardwareFlowControl() const override
		{
			return _hardware_flow_control;
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
		virtual int32_t Read(base::Span const &span) override;

		///
		/// @brief 调用后临时启动 DMA 进行一次发送。
		/// @param span
		///
		virtual void Write(base::ReadOnlySpan const &span) override;

		virtual void Close() override;
	};
} // namespace bsp

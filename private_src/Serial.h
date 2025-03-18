#pragma once
#include <base/define.h>
#include <base/task/IMutex.h>
#include <bsp-interface/di/dma.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <bsp-interface/serial/ISerial.h>
#include <hal.h>

namespace bsp
{
	class Serial :
		public bsp::serial::ISerial
	{
	private:
		Serial() = default;

		bool _have_begun = false;
		UART_HandleTypeDef _uart_handle{};
		std::shared_ptr<bsp::IBinarySemaphore> _sending_completion_signal = bsp::di::task::CreateBinarySemaphore();
		std::shared_ptr<bsp::IBinarySemaphore> _receiving_completion_signal = bsp::di::task::CreateBinarySemaphore();
		std::shared_ptr<base::IMutex> _read_lock = base::CreateIMutex();

		bsp::dma::IDmaChannel *_rx_dma_channel = nullptr;
		bsp::dma::IDmaChannel *_tx_dma_channel = nullptr;

		bsp::serial::property::Direction _direction;
		uint32_t _baud_rate;
		uint8_t _data_bits;
		bsp::serial::property::Parity _parity;
		bsp::serial::property::StopBits _stop_bits;
		bsp::serial::property::HardwareFlowControl _hardware_flow_control;

#pragma region 初始化
		void InitializeGpio();
		void InitializeDma();
		void InitializeUart();
		void InitializeInterrupt();
#pragma endregion

		/// @brief 通过串口句柄和 DMA 寄存器，获取当前 DMA 接收了多少个字节。
		/// @return
		int32_t HaveRead();

#pragma region 被中断处理函数回调的函数
		static_function void OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos);
		static_function void OnSendCompleteCallback(UART_HandleTypeDef *huart);
		static_function void OnReadTimeout(UART_HandleTypeDef *huart);
#pragma endregion

		void SetReadTimeoutByBaudCount(uint32_t value);

	public:
		static_function Serial &Instance();

		std::string Name() override
		{
			return "serial";
		}

#pragma region Stream
		/// @brief 调用后临时启动 DMA 接收一次数据。
		/// @note 本类没有缓冲机制，所以上层应用如果调用 Read 不及时，会丢失数据。
		/// @note 因为调用一次 Read 具有一定开销，需要设置寄存器，使能中断，设置一些
		/// 状态变量。所以为了提高效率，每次调用 Read 时传入的 buffer 适当大一些，
		/// 并且 count 大一些。
		///
		/// @param span
		/// @return
		virtual int32_t Read(base::Span const &span) override;

		/// @brief 调用后临时启动 DMA 进行一次发送。
		/// @param span
		virtual void Write(base::ReadOnlySpan const &span) override;

		void Close() override;
#pragma endregion

		/// @brief 打开串口。
		/// @param direction 串口数据方向。可以选择只发、只收、收发。
		/// @param baud_rate 波特率。
		/// @param data_bits 数据位位数。
		/// @param parity 奇偶校验。
		/// @param stop_bits 停止位位数。
		/// @param hardware_flow_control 硬件流控。
		virtual void Open(bsp::serial::property::Direction direction,
						  bsp::serial::property::BaudRate const &baud_rate,
						  bsp::serial::property::DataBits const &data_bits,
						  bsp::serial::property::Parity parity,
						  bsp::serial::property::StopBits stop_bits,
						  bsp::serial::property::HardwareFlowControl hardware_flow_control) override;

#pragma region 串口属性

		/// @brief 数据传输方向
		/// @return
		virtual bsp::serial::property::Direction Direction() override
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
		virtual bsp::serial::property::Parity Parity() const override
		{
			return _parity;
		}

		/// @brief 停止位个数。
		/// @return
		virtual bsp::serial::property::StopBits StopBits() const override
		{
			return _stop_bits;
		}

		/// @brief 硬件流控。
		/// @return
		virtual bsp::serial::property::HardwareFlowControl HardwareFlowControl() const override
		{
			return _hardware_flow_control;
		}

#pragma endregion
	};
} // namespace bsp

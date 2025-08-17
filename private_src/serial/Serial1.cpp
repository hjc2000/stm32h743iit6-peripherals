#include "Serial1.h"
#include "base/embedded/interrupt/interrupt.h"
#include "base/embedded/serial/serial_handle.h"
#include "base/IDisposable.h"
#include "base/stream/ReadOnlySpan.h"
#include "base/string/define.h"
#include "base/task/Mutex.h"
#include <functional>
#include <stdexcept>

/* #region 中断处理函数 */

namespace
{
	std::function<void()> _uart1_isr;
	std::function<void()> _dma1_stream0_isr;
	std::function<void()> _dma1_stream1_isr;

} // namespace

extern "C"
{
	void USART1_IRQHandler()
	{
		try
		{
			_uart1_isr();
		}
		catch (...)
		{
		}
	}

	void DMA1_Stream0_IRQHandler()
	{
		try
		{
			_dma1_stream0_isr();
		}
		catch (...)
		{
		}
	}

	void DMA1_Stream1_IRQHandler()
	{
		try
		{
			_dma1_stream1_isr();
		}
		catch (...)
		{
		}
	}
}

/* #endregion */

/* #region 初始化 */

void bsp::Serial1::InitializeGpio()
{
	__HAL_RCC_USART1_CLK_ENABLE();

	// 发送引脚 PA9
	_pa9.InitializeAsAlternateFunctionMode(GPIO_AF7_USART1,
										   base::gpio::PullMode::PullUp,
										   base::gpio::DriveMode::PushPull);

	// 接收引脚 PA10
	_pa10.InitializeAsAlternateFunctionMode(GPIO_AF7_USART1,
											base::gpio::PullMode::PullUp,
											base::gpio::DriveMode::PushPull);
}

void bsp::Serial1::InitializeRxDma()
{
	__HAL_RCC_DMA1_CLK_ENABLE();

	_rx_dma_handle.Instance = DMA1_Stream1;
	_rx_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	_rx_dma_handle.Init.Request = DMA_REQUEST_USART1_RX;
	_rx_dma_handle.Init.Mode = DMA_NORMAL;
	_rx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	_rx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	_rx_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
	_rx_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	_rx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
	_rx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
	_rx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	_rx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	_rx_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
	HAL_DMA_Init(&_rx_dma_handle);

	_handle_context._uart_handle.hdmarx = &_rx_dma_handle;
	_rx_dma_handle.Parent = &_handle_context._uart_handle;
}

void bsp::Serial1::InitializeTxDma()
{
	__HAL_RCC_DMA1_CLK_ENABLE();

	_tx_dma_handle.Instance = DMA1_Stream0;
	_tx_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
	_tx_dma_handle.Init.Request = DMA_REQUEST_USART1_TX;
	_tx_dma_handle.Init.Mode = DMA_NORMAL;
	_tx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	_tx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	_tx_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
	_tx_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	_tx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
	_tx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
	_tx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	_tx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	_tx_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
	HAL_DMA_Init(&_tx_dma_handle);

	_handle_context._uart_handle.hdmatx = &_tx_dma_handle;
	_tx_dma_handle.Parent = &_handle_context._uart_handle;
}

void bsp::Serial1::InitializeUart()
{
	_handle_context._uart_handle.Instance = USART1;

	switch (_direction)
	{
	case base::serial::Direction::RX:
		{
			_handle_context._uart_handle.Init.Mode = UART_MODE_RX;
			break;
		}
	case base::serial::Direction::TX:
		{
			_handle_context._uart_handle.Init.Mode = UART_MODE_TX;
			break;
		}
	default:
	case base::serial::Direction::RX_TX:
		{
			_handle_context._uart_handle.Init.Mode = UART_MODE_TX_RX;
			break;
		}
	}

	_handle_context._uart_handle.Init.BaudRate = _baud_rate;

	switch (_data_bits)
	{
	default:
	case 8:
		{
			_handle_context._uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
			break;
		}
	case 9:
		{
			_handle_context._uart_handle.Init.WordLength = UART_WORDLENGTH_9B;
			break;
		}
	}

	switch (_parity)
	{
	default:
	case base::serial::Parity::None:
		{
			_handle_context._uart_handle.Init.Parity = UART_PARITY_NONE;
			break;
		}
	case base::serial::Parity::Even:
		{
			_handle_context._uart_handle.Init.Parity = UART_PARITY_EVEN;
			break;
		}
	case base::serial::Parity::Odd:
		{
			_handle_context._uart_handle.Init.Parity = UART_PARITY_ODD;
			break;
		}
	}

	switch (_stop_bits)
	{
	default:
	case base::serial::StopBits::One:
		{
			_handle_context._uart_handle.Init.StopBits = UART_STOPBITS_1;
			break;
		}
	case base::serial::StopBits::Two:
		{
			_handle_context._uart_handle.Init.StopBits = UART_STOPBITS_2;
			break;
		}
	}

	switch (_hardware_flow_control)
	{
	default:
	case base::serial::HardwareFlowControl::None:
		{
			_handle_context._uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			break;
		}
	case base::serial::HardwareFlowControl::RTS:
		{
			_handle_context._uart_handle.Init.HwFlowCtl = UART_HWCONTROL_RTS;
			break;
		}
	case base::serial::HardwareFlowControl::CTS:
		{
			_handle_context._uart_handle.Init.HwFlowCtl = UART_HWCONTROL_CTS;
			break;
		}
	case base::serial::HardwareFlowControl::RTS_CTS:
		{
			_handle_context._uart_handle.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
			break;
		}
	}

	// 16 倍过采样。每一个位被采样 16 次，最后可能是根据平均值来确定它的电平。
	_handle_context._uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;

	/**
	 * 每一次采样是使用 1 位的 ADC 采样还是使用 3 位的 ADC 采样。
	 * UART_ONE_BIT_SAMPLE_DISABLE 表示使用 3 位的 ADC 采样。
	 */
	_handle_context._uart_handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

	// 对 PCLK 的分频系数。
	_handle_context._uart_handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	_handle_context._uart_handle.MspInitCallback = nullptr;
	HAL_UART_Init(&_handle_context._uart_handle);

	/*
	 * HAL_UART_Init 函数会把中断处理函数中回调的函数都设为默认的，所以必须在 HAL_UART_Init
	 * 之后对函数指针赋值。
	 */
	_handle_context._uart_handle.RxEventCallback = OnReceiveEventCallback;
	_handle_context._uart_handle.TxCpltCallback = OnSendCompleteCallback;
	_handle_context._uart_handle.ErrorCallback = OnReadTimeout;

	// 超时在串口初始化后设置才有效
	SetReadTimeoutByBaudCount(base::serial::frames_baud_count(*this, 2));
}

void bsp::Serial1::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::USART1_IRQn));
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn));
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn));

	_uart1_isr = [this]()
	{
		HAL_UART_IRQHandler(&_handle_context._uart_handle);
	};

	_dma1_stream0_isr = [this]()
	{
		HAL_DMA_IRQHandler(_handle_context._uart_handle.hdmatx);
	};

	_dma1_stream1_isr = [this]()
	{
		HAL_DMA_IRQHandler(_handle_context._uart_handle.hdmarx);
	};

	base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::USART1_IRQn), 10);
	base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn), 10);
	base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn), 10);
}

/* #endregion */

int32_t bsp::Serial1::HaveRead()
{
	// 经过测试，使用 HAL_UART_Receive_DMA 函数配合硬件接收超时时，一定要这么计算
	// 已经接收的长度，不能使用
	//
	// _handle_context._uart_handle.RxXferSize - _handle_context._uart_handle.RxXferCount
	//
	// 似乎是因为 HAL 库在超时时算作错误，然后错误会把 RxXferCount 赋值为 0.
	return _handle_context._uart_handle.RxXferSize - __HAL_DMA_GET_COUNTER(&_rx_dma_handle);
}

/* #region 被中断处理函数回调的函数 */

void bsp::Serial1::OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos)
{
	handle_context *context = reinterpret_cast<handle_context *>(huart);
	context->_self->_receiving_completion_signal.ReleaseFromIsr();
}

void bsp::Serial1::OnSendCompleteCallback(UART_HandleTypeDef *huart)
{
	handle_context *context = reinterpret_cast<handle_context *>(huart);
	context->_self->_sending_completion_signal.ReleaseFromIsr();
}

void bsp::Serial1::OnReadTimeout(UART_HandleTypeDef *huart)
{
	handle_context *context = reinterpret_cast<handle_context *>(huart);
	context->_self->_receiving_completion_signal.ReleaseFromIsr();
}

/* #endregion */

void bsp::Serial1::SetReadTimeoutByBaudCount(uint32_t value)
{
	if (value > 0)
	{
		HAL_UART_ReceiverTimeout_Config(&_handle_context._uart_handle, value);
		HAL_UART_EnableReceiverTimeout(&_handle_context._uart_handle);
	}
	else
	{
		HAL_UART_DisableReceiverTimeout(&_handle_context._uart_handle);
	}
}

bsp::Serial1::~Serial1()
{
	Close();
}

int64_t bsp::Serial1::Read(base::Span const &span)
{
	if (_closed)
	{
		return 0;
	}

	if (span.Size() > UINT16_MAX)
	{
		throw std::invalid_argument{"count 太大"};
	}

	base::task::MutexGuard l{_read_lock};
	while (true)
	{
		if (_closed)
		{
			return 0;
		}

		{
			base::interrupt::GlobalInterruptionGuard g;

			// HAL_UART_Receive_DMA
			// HAL_UARTEx_ReceiveToIdle_DMA
			HAL_UART_Receive_DMA(&_handle_context._uart_handle, span.Buffer(), span.Size());

			// 通过赋值为空指针，把传输半满回调给禁用，不然接收的数据较长，超过缓冲区一半时，
			// 即使是一次性接收的，UART 也会回调 OnReceiveEventCallback 两次。
			_handle_context._uart_handle.hdmarx->XferHalfCpltCallback = nullptr;
		}

		_receiving_completion_signal.Acquire();
		if (HaveRead() > 0)
		{
			return HaveRead();
		}
	}
}

void bsp::Serial1::Write(base::ReadOnlySpan const &span)
{
	if (_closed)
	{
		throw base::ObjectDisposedException{CODE_POS_STR + "串口已关闭，无法写入。"};
	}

	base::task::MutexGuard l{_write_lock};

	HAL_StatusTypeDef ret = HAL_UART_Transmit_DMA(&_handle_context._uart_handle,
												  span.Buffer(),
												  span.Size());

	if (ret != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "发送失败"};
	}

	_sending_completion_signal.Acquire();
}

void bsp::Serial1::Start(base::serial::Direction direction,
						 base::serial::BaudRate const &baud_rate,
						 base::serial::DataBits const &data_bits,
						 base::serial::Parity parity,
						 base::serial::StopBits stop_bits,
						 base::serial::HardwareFlowControl hardware_flow_control)
{
	_direction = direction;
	_baud_rate = baud_rate.Value();
	_data_bits = data_bits.Value();
	_parity = parity;
	_stop_bits = stop_bits;
	_hardware_flow_control = hardware_flow_control;

	InitializeGpio();
	InitializeRxDma();
	InitializeTxDma();
	InitializeUart();
	InitializeInterrupt();
}

void bsp::Serial1::Close()
{
	if (_closed)
	{
		return;
	}

	_closed = true;
	_receiving_completion_signal.Release();
	_sending_completion_signal.Release();

	HAL_UART_DMAStop(&_handle_context._uart_handle);

	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::USART1_IRQn));
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream0_IRQn));
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::DMA1_Stream1_IRQn));
	_uart1_isr = nullptr;
	_dma1_stream0_isr = nullptr;
	_dma1_stream1_isr = nullptr;
}

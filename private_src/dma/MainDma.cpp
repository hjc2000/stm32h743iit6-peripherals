#include "MainDma.h" // IWYU pragma: keep
#include "base/embedded/cache/cache.h"
#include "base/embedded/interrupt/interrupt.h"
#include "mdma_define.h"
#include <functional>

/* #region 中断服务函数 */

namespace
{
	std::function<void()> _dma_isr;

}

extern "C"
{
	void MDMA_IRQHandler(void)
	{
		if (_dma_isr != nullptr)
		{
			_dma_isr();
		}
	}
}

/* #endregion */

void bsp::MainDma::InitializeInterrupt()
{
	base::interrupt::disable_interrupt(static_cast<uint32_t>(IRQn_Type::MDMA_IRQn));

	_dma_isr = [this]()
	{
		HAL_MDMA_IRQHandler(&_handle_context._handle);
	};

	base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::MDMA_IRQn), 10);
}

void bsp::MainDma::Initialize()
{
	Initialize(1);
}

void bsp::MainDma::InitializeCallback()
{
	_handle_context._handle.XferCpltCallback = [](MDMA_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->OnCompleteCallback();
	};

	_handle_context._handle.XferErrorCallback = [](MDMA_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->OnErrorCallback();
	};

	_handle_context._handle.XferAbortCallback = [](MDMA_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->OnAbortCallback();
	};
}

void bsp::MainDma::Initialize(size_t align)
{
	_align = align;
	__HAL_RCC_MDMA_CLK_ENABLE();

	_handle_context._handle.Instance = MDMA_Channel0;

	// 软件请求触发
	_handle_context._handle.Init.Request = MDMA_REQUEST_SW;

	// 块传输。
	// 这样可以每次启动传输都更改源地址、目的地址、传输大小。
	_handle_context._handle.Init.TransferTriggerMode = MDMA_BLOCK_TRANSFER;

	// 传输优先级。
	_handle_context._handle.Init.Priority = MDMA_PRIORITY_VERY_HIGH;

	// 保留为小端序。
	// 因为 ARM 本身是小端序的，所以叫作保留。
	// 保留的意思是保留原数据的字节序，不进行转换。
	_handle_context._handle.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;

	// 源地址的数据对齐。
	_handle_context._handle.Init.SourceInc = bsp::mdma::source_increase_size_to_define(align, true);

	// 目标地址的数据对齐。
	_handle_context._handle.Init.DestinationInc = bsp::mdma::destination_increase_size_to_define(align, true);

	// 源地址每个数据单元的大小。
	_handle_context._handle.Init.SourceDataSize = bsp::mdma::source_data_size_to_define(align);

	// 目标地址每个数据单元的大小。
	_handle_context._handle.Init.DestDataSize = bsp::mdma::destination_data_size_to_define(align);

	// 源地址和目的地址的数据单元大小不一样是，采用的对齐方式。
	//
	// 可以选择
	// 		1. 打包
	// 		2. 右对齐
	// 		3. 有符号地右对齐
	// 		4. 左对齐
	//
	// 打包的意思是将多个小单元合并成一个大单元。
	_handle_context._handle.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;

	// 传输缓冲区大小。
	//
	// TransferTriggerMode 设置为 MDMA_BUFFER_TRANSFER 时每次请求搬运这么多数据。
	//
	// TransferTriggerMode 设置为 MDMA_BLOCK_TRANSFER 时，决定了内部 FIFO 的大小，
	// 后续设置的突发大小不能超过此值。
	_handle_context._handle.Init.BufferTransferLength = 128;

	// 从源地址读取数据时的突发大小。
	//
	// SINGLE 表示一次只读取一个数据，数据的字节数为 SourceDataSize.
	_handle_context._handle.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;

	// 向目标地址写入数据时的突发大小。
	//
	// SINGLE 表示一次只写入一个数据，数据的字节数为 DestDataSize.
	_handle_context._handle.Init.DestBurst = MDMA_DEST_BURST_SINGLE;

	// HAL_MDMA_Start_IT 函数的参数中可以传入要拷贝的块的个数。当传入的拷贝的块的个数
	// 大于 1 时，拷贝完一个块后，需要将起始指针移动到下一个块的起始地址。
	//
	// 这里是定义下一个块的起始地址相对于当前块的起始地址的偏移量。偏移量的单位是字节。
	// 对于紧凑的数组，偏移量就是数组元素的大小，也即字节数。
	// 对于结构体字段这种不紧凑的，有间隙的，关系为：
	// 		偏移量 = 字段大小 + 字段之间的间隔。
	//
	// 注意，单位是字节，不是元素的大小。即不是 SourceDataSize.
	//
	// SourceBlockAddressOffset 和 DestBlockAddressOffset 都设置为 0,
	// 这样即使 HAL_MDMA_Start_IT 中要拷贝的块的个数不小心设置为大于 1 的值，
	// 也只是对着相同内存拷贝多次而已，是幂等的。
	_handle_context._handle.Init.SourceBlockAddressOffset = 0;

	// 参考 SourceBlockAddressOffset 的注释。
	_handle_context._handle.Init.DestBlockAddressOffset = 0;

	HAL_StatusTypeDef result = HAL_MDMA_Init(&_handle_context._handle);
	if (result != HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "初始化 MDMA 失败。"};
	}

	InitializeInterrupt();
	InitializeCallback();
}

void bsp::MainDma::Copy(uint8_t const *begin, uint8_t const *end, uint8_t *dst)
{
	base::task::MutexGuard g{_lock};

	base::cache::clean_d_cache(begin, end - begin);

	// 如果 CPU 修改了 dst 处的数据，要先清理缓存，否则等会儿 DMA 向 RAM 写入数据后
	// 无效化缓存，会把 CPU 修改的脏数据丢弃，让 DMA 写入 RAM 的数据强行覆盖到缓存中。
	//
	// 所以这里要清理缓存比较保险。
	base::cache::clean_d_cache(dst, end - begin);

	_is_error = false;
	_is_abort = false;

	HAL_StatusTypeDef result = HAL_MDMA_Start_IT(&_handle_context._handle,
												 reinterpret_cast<uint32_t>(begin),
												 reinterpret_cast<uint32_t>(dst),
												 static_cast<uint32_t>((end - begin) / _align),
												 1);

	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{CODE_POS_STR + "DMA 启动失败。"};
	}

	_complete_signal.Acquire();
	if (_is_error)
	{
		throw std::runtime_error{CODE_POS_STR + "DMA 传输发生错误。"};
	}

	if (_is_abort)
	{
		throw std::runtime_error{CODE_POS_STR + "DMA 传输被中止。"};
	}

	base::cache::invalidate_d_cache(dst, end - begin);
}

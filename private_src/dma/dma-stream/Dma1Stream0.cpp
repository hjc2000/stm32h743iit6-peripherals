#include "Dma1Stream0.h"
#include "base/define.h"

PREINIT(bsp::Dma1Stream0::Instance);

bsp::Dma1Stream0 &bsp::Dma1Stream0::Instance()
{
	static bsp::Dma1Stream0 o{};
	return o;
}

std::string bsp::Dma1Stream0::Name() const
{
	return "dma1_stream0";
}

void bsp::Dma1Stream0::OpenAsPeripheralToMemoryMode(void *parent,
													bsp::dma::PeripheralIncrement peripheral_increment,
													bsp::dma::MemoryIncrement const &memory_increment,
													bsp::dma::PeripheralDataAlignment const &peripheral_data_alignment,
													bsp::dma::MemoryDataAlignment const &memory_data_alignment,
													bsp::dma::Priority priority,
													std::string const &request)
{
	if (_is_open)
	{
		throw std::runtime_error{"已经打开，要先关闭才能再次打开"};
	}

	_is_open = true;

	__HAL_RCC_DMA1_CLK_ENABLE();
	_dma_handle.Instance = DMA1_Stream0;

	_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;

	InitializeDmaProperty(peripheral_increment,
						  memory_increment,
						  peripheral_data_alignment,
						  memory_data_alignment,
						  priority,
						  request);

	HAL_DMA_Init(&_dma_handle);
	LinkDmaToParent(parent);
}

void bsp::Dma1Stream0::OpenAsMemoryToPeripheralMode(void *parent,
													bsp::dma::PeripheralIncrement peripheral_increment,
													bsp::dma::MemoryIncrement const &memory_increment,
													bsp::dma::PeripheralDataAlignment const &peripheral_data_alignment,
													bsp::dma::MemoryDataAlignment const &memory_data_alignment,
													bsp::dma::Priority priority,
													std::string const &request)
{
	if (_is_open)
	{
		throw std::runtime_error{"已经打开，要先关闭才能再次打开"};
	}

	_is_open = true;

	__HAL_RCC_DMA1_CLK_ENABLE();
	_dma_handle.Instance = DMA1_Stream0;

	_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;

	InitializeDmaProperty(peripheral_increment,
						  memory_increment,
						  peripheral_data_alignment,
						  memory_data_alignment,
						  priority,
						  request);

	HAL_DMA_Init(&_dma_handle);
	LinkDmaToParent(parent);
}

void bsp::Dma1Stream0::OpenAsMomoryToMemoryMode(void *parent,
												bsp::dma::PeripheralIncrement peripheral_increment,
												bsp::dma::MemoryIncrement const &memory_increment,
												bsp::dma::PeripheralDataAlignment const &peripheral_data_alignment,
												bsp::dma::MemoryDataAlignment const &memory_data_alignment,
												bsp::dma::Priority priority,
												std::string const &request)
{
	if (_is_open)
	{
		throw std::runtime_error{"已经打开，要先关闭才能再次打开"};
	}

	_is_open = true;

	__HAL_RCC_DMA1_CLK_ENABLE();
	_dma_handle.Instance = DMA1_Stream0;

	_dma_handle.Init.Direction = DMA_MEMORY_TO_MEMORY;

	InitializeDmaProperty(peripheral_increment,
						  memory_increment,
						  peripheral_data_alignment,
						  memory_data_alignment,
						  priority,
						  request);

	HAL_DMA_Init(&_dma_handle);
	LinkDmaToParent(parent);
}

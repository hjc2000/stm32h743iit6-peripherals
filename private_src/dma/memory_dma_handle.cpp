#include "memory_dma_handle.h" // IWYU pragma: keep
#include "MainDma.h"
#include "MemoryDma1.h"

std::shared_ptr<base::memory_dma::memory_dma_handle> base::memory_dma::open(uint32_t id)
{
	switch (id)
	{
	case 1:
		{
			return std::shared_ptr<base::memory_dma::memory_dma_handle>{new bsp::MainDma{}};
		}
	case 2:
		{
			return std::shared_ptr<base::memory_dma::memory_dma_handle>{new bsp::MemoryDma1{}};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法 ID."};
		}
	}
}

void base::memory_dma::initialize(base::memory_dma::memory_dma_handle &self)
{
	self.Initialize();
}

void base::memory_dma::initialize(base::memory_dma::memory_dma_handle &self, size_t align)
{
	self.Initialize(align);
}

void base::memory_dma::copy(base::memory_dma::memory_dma_handle &self,
							uint8_t const *begin,
							uint8_t const *end,
							uint8_t *dst)
{
	self.Copy(begin, end, dst);
}

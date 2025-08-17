#include "memory_dma_handle.h" // IWYU pragma: keep
#include "MemoryDma1.h"

std::shared_ptr<base::memory_dma::memory_dma_handle> base::memory_dma::open(uint32_t id)
{
	return std::shared_ptr<base::memory_dma::memory_dma_handle>{new bsp::MemoryDma1{}};
}

void base::memory_dma::initialize(base::memory_dma::memory_dma_handle &self)
{
	self.Initialize();
}

void base::memory_dma::copy(base::memory_dma::memory_dma_handle &self,
							uint8_t const *begin,
							uint8_t const *end,
							uint8_t *dst)
{
	self.Copy(begin, end, dst);
}

#pragma once
#include "base/embedded/dma/memory_dma_handle.h"
#include <cstddef>

class base::memory_dma::memory_dma_handle
{
public:
	virtual ~memory_dma_handle() = default;

	virtual void Initialize() = 0;
	virtual void Initialize(size_t align) = 0;

	virtual void Copy(uint8_t const *begin, uint8_t const *end, uint8_t *dst) = 0;
};

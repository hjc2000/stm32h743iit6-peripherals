#include "base/embedded/cache/cache.h"
#include "hal.h"
#include <cstdint>

void base::cache::enable()
{
	SCB_EnableICache();
	SCB_EnableDCache();
	SCB->CACR |= 1 << 2;
}

void base::cache::disable()
{
	SCB_DisableICache();
	SCB_DisableDCache();
}

void base::cache::invalidate_d_cache()
{
	SCB_InvalidateDCache();
}

void base::cache::invalidate_d_cache(void *address, size_t size)
{
	SCB_InvalidateDCache_by_Addr(address, size);
}

void base::cache::clean_d_cache()
{
	SCB_CleanDCache();
}

void base::cache::clean_d_cache(void *address, size_t size)
{
	SCB_CleanDCache_by_Addr(reinterpret_cast<uint32_t *>(address), size);
}

void base::cache::clean_and_invalidate_d_cache()
{
	SCB_CleanInvalidateDCache();
}

void base::cache::clean_and_invalidate_d_cache(void *address, size_t size)
{
	SCB_CleanInvalidateDCache_by_Addr(reinterpret_cast<uint32_t *>(address), size);
}

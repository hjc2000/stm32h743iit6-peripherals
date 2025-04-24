#include "base/peripheral/cache/cache.h"
#include "hal.h"

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

///
/// @brief 清除 D-Cache。
///
/// @note 即将原本从内存中读取到 D-Cache 中，并且已经修改过的 D-Cache 的数据写入
/// 内存中而不是仅仅停留在 D-Cache 中。
///
void base::cache::clean_d_cache()
{
	SCB_CleanDCache();
}

void base::cache::clean_and_invalidate_d_cache()
{
	SCB_CleanInvalidateDCache();
}

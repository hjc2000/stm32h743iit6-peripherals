#include "base/embedded/cortex/mpu.h"
#include "base/embedded/cortex/MemoryType.h"
#include "base/string/define.h"
#include "hal.h"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace
{
	std::bitset<16> _mpu_region_bit_set{};

	constexpr uint32_t region_number_to_define(uint32_t value)
	{
		switch (value)
		{
		case 0:
			{
				return MPU_REGION_NUMBER0;
			}
		case 1:
			{
				return MPU_REGION_NUMBER1;
			}
		case 2:
			{
				return MPU_REGION_NUMBER2;
			}
		case 3:
			{
				return MPU_REGION_NUMBER3;
			}
		case 4:
			{
				return MPU_REGION_NUMBER4;
			}
		case 5:
			{
				return MPU_REGION_NUMBER5;
			}
		case 6:
			{
				return MPU_REGION_NUMBER6;
			}
		case 7:
			{
				return MPU_REGION_NUMBER7;
			}
		case 8:
			{
				return MPU_REGION_NUMBER8;
			}
		case 9:
			{
				return MPU_REGION_NUMBER9;
			}
		case 10:
			{
				return MPU_REGION_NUMBER10;
			}
		case 11:
			{
				return MPU_REGION_NUMBER11;
			}
		case 12:
			{
				return MPU_REGION_NUMBER12;
			}
		case 13:
			{
				return MPU_REGION_NUMBER13;
			}
		case 14:
			{
				return MPU_REGION_NUMBER14;
			}
		case 15:
			{
				return MPU_REGION_NUMBER15;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法区域。"};
			}
		}
	}

	constexpr size_t size_to_define(size_t value)
	{
		switch (value)
		{
		case static_cast<size_t>(32):
			{
				return MPU_REGION_SIZE_32B;
			}
		case static_cast<size_t>(64):
			{
				return MPU_REGION_SIZE_64B;
			}
		case static_cast<size_t>(128):
			{
				return MPU_REGION_SIZE_128B;
			}
		case static_cast<size_t>(256):
			{
				return MPU_REGION_SIZE_256B;
			}
		case static_cast<size_t>(512):
			{
				return MPU_REGION_SIZE_512B;
			}
		case static_cast<size_t>(1) * 1024:
			{
				return MPU_REGION_SIZE_1KB;
			}
		case static_cast<size_t>(2) * 1024:
			{
				return MPU_REGION_SIZE_2KB;
			}
		case static_cast<size_t>(4) * 1024:
			{
				return MPU_REGION_SIZE_4KB;
			}
		case static_cast<size_t>(8) * 1024:
			{
				return MPU_REGION_SIZE_8KB;
			}
		case static_cast<size_t>(16) * 1024:
			{
				return MPU_REGION_SIZE_16KB;
			}
		case static_cast<size_t>(32) * 1024:
			{
				return MPU_REGION_SIZE_32KB;
			}
		case static_cast<size_t>(64) * 1024:
			{
				return MPU_REGION_SIZE_64KB;
			}
		case static_cast<size_t>(128) * 1024:
			{
				return MPU_REGION_SIZE_128KB;
			}
		case static_cast<size_t>(512) * 1024:
			{
				return MPU_REGION_SIZE_512KB;
			}
		case static_cast<size_t>(1) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_1MB;
			}
		case static_cast<size_t>(2) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_2MB;
			}
		case static_cast<size_t>(4) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_4MB;
			}
		case static_cast<size_t>(8) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_8MB;
			}
		case static_cast<size_t>(16) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_16MB;
			}
		case static_cast<size_t>(32) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_32MB;
			}
		case static_cast<size_t>(64) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_64MB;
			}
		case static_cast<size_t>(128) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_128MB;
			}
		case static_cast<size_t>(512) * 1024 * 1024:
			{
				return MPU_REGION_SIZE_512MB;
			}
		case static_cast<size_t>(1) * 1024 * 1024 * 1024:
			{
				return MPU_REGION_SIZE_1GB;
			}
		case static_cast<size_t>(2) * 1024 * 1024 * 1024:
			{
				return MPU_REGION_SIZE_2GB;
			}
		case static_cast<size_t>(4) * 1024 * 1024 * 1024:
			{
				return MPU_REGION_SIZE_4GB;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法大小。"};
			}
		}
	}

	constexpr void configure_memory_type(MPU_Region_InitTypeDef &configuration, base::cortex::MemoryType value)
	{
		switch (value)
		{
		case base::cortex::MemoryType::Device:
			{
				configuration.TypeExtField = MPU_TEX_LEVEL0;
				configuration.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
				configuration.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
				configuration.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
				break;
			}
		case base::cortex::MemoryType::DeviceWithWritingBuffer:
			{
				configuration.TypeExtField = MPU_TEX_LEVEL0;
				configuration.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
				configuration.IsBufferable = MPU_ACCESS_BUFFERABLE;
				configuration.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
				break;
			}
		case base::cortex::MemoryType::Normal:
			{
				configuration.TypeExtField = MPU_TEX_LEVEL1;
				configuration.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
				configuration.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
				configuration.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
				break;
			}
		case base::cortex::MemoryType::NormalWithCache_WriteThrough:
			{
				configuration.TypeExtField = MPU_TEX_LEVEL1;
				configuration.IsCacheable = MPU_ACCESS_CACHEABLE;
				configuration.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
				configuration.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
				break;
			}
		case base::cortex::MemoryType::NormalWithCache_WriteBack:
			{
				configuration.TypeExtField = MPU_TEX_LEVEL1;
				configuration.IsCacheable = MPU_ACCESS_CACHEABLE;
				configuration.IsBufferable = MPU_ACCESS_BUFFERABLE;
				configuration.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
				break;
			}
		default:
			{
				throw std::invalid_argument{CODE_POS_STR + "非法内存类型。"};
			}
		}
	}

	class MpuGuard
	{
	public:
		MpuGuard()
		{
			HAL_MPU_Disable();
		}

		~MpuGuard()
		{
			HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
		}
	};

} // namespace

void base::cortex::mpu::configure(uint32_t region_number,
								  size_t base_address,
								  size_t size,
								  base::cortex::MemoryType memory_type)
{
	if (_mpu_region_bit_set[region_number])
	{
		std::string message = CODE_POS_STR;
		message += "区域 " + std::to_string(region_number) + " 已配置。";
		throw std::runtime_error{message};
	}

	MpuGuard g{};
	MPU_Region_InitTypeDef mpu_region_init_handle{};
	mpu_region_init_handle.Enable = MPU_REGION_ENABLE;
	mpu_region_init_handle.Number = region_number_to_define(region_number);
	mpu_region_init_handle.BaseAddress = base_address;
	mpu_region_init_handle.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
	mpu_region_init_handle.Size = size_to_define(size);
	mpu_region_init_handle.SubRegionDisable = 0x00;
	mpu_region_init_handle.AccessPermission = MPU_REGION_FULL_ACCESS;
	configure_memory_type(mpu_region_init_handle, memory_type);
	HAL_MPU_ConfigRegion(&mpu_region_init_handle);
}

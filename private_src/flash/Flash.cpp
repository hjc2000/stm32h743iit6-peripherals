#include "Flash.h"
#include "base/define.h"
#include "base/embedded/interrupt/interrupt.h"
#include <stdexcept>

extern "C"
{
	void FLASH_IRQHandler()
	{
		HAL_FLASH_IRQHandler();
	}

	void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue)
	{
		bsp::Flash::Instance()._operation_failed = false;
		bsp::Flash::Instance()._operation_completed.ReleaseFromIsr();
	}

	void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
	{
		bsp::Flash::Instance()._operation_failed = true;
		bsp::Flash::Instance()._operation_completed.ReleaseFromIsr();
	}
}

bsp::Flash::Flash()
{
	base::interrupt::enable_interrupt(static_cast<uint32_t>(IRQn_Type::FLASH_IRQn));
}

uint32_t bsp::Flash::SectorIndexToDefine(int32_t index)
{
	switch (index)
	{
	case 0:
		{
			return FLASH_SECTOR_0;
		}
	case 1:
		{
			return FLASH_SECTOR_1;
		}
	case 2:
		{
			return FLASH_SECTOR_2;
		}
	case 3:
		{
			return FLASH_SECTOR_3;
		}
	case 4:
		{
			return FLASH_SECTOR_4;
		}
	case 5:
		{
			return FLASH_SECTOR_5;
		}
	case 6:
		{
			return FLASH_SECTOR_6;
		}
	case 7:
		{
			return FLASH_SECTOR_7;
		}
	default:
		{
			throw std::out_of_range{"index 超出范围"};
		}
	}
}

PREINIT(bsp::Flash::Instance);

bsp::Flash &bsp::Flash::Instance()
{
	static Flash o{};
	return o;
}

#pragma region 锁

void bsp::Flash::Lock()
{
	HAL_StatusTypeDef result = HAL_FLASH_Lock();
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"锁定 flash 失败"};
	}
}

void bsp::Flash::Unlock()
{
	HAL_StatusTypeDef result = HAL_FLASH_Unlock();
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"解锁 flash 失败"};
	}
}

#pragma endregion

#pragma region flash 信息，规格参数

std::string bsp::Flash::Name()
{
	// 片内 flash
	return "internal-flash";
}

size_t bsp::Flash::SectorSize() const
{
	return static_cast<size_t>(128) << 10;
}

int32_t bsp::Flash::SectorCount() const
{
	return 8;
}

size_t bsp::Flash::BaseAddress() const
{
	return 0x08100000;
}

int32_t bsp::Flash::ProgrammingSize() const
{
	return 32;
}

#pragma endregion

#pragma region 擦除

void bsp::Flash::Erase()
{
	base::UnlockGuard ul{*this};
	FLASH_EraseInitTypeDef def;

	// 擦除类型为 bank 擦除
	def.TypeErase = FLASH_TYPEERASE_MASSERASE;

	// 选择 bank
	def.Banks = FLASH_BANK_2;

	// 每次擦除 64 位，直到擦除整个 bank。
	// 越高的电压擦除时并行的位数就越多。
	def.VoltageRange = FLASH_VOLTAGE_RANGE_4;

	/* 擦除到某个扇区的时候如果发生错误，会停止擦除，将扇区索引赋值给 error_sector_index，
	 * 然后返回。
	 */
	HAL_StatusTypeDef result = HAL_FLASHEx_Erase_IT(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动擦除流程失败"};
	}

	_operation_completed.Acquire();
	if (_operation_failed)
	{
		throw std::runtime_error{"擦除流程结束，出错了"};
	}

	SCB_CleanInvalidateDCache();
}

void bsp::Flash::EraseSector(int32_t sector_index)
{
	base::UnlockGuard ul{*this};
	FLASH_EraseInitTypeDef def;

	// 擦除类型为扇区擦除
	def.TypeErase = FLASH_TYPEERASE_SECTORS;

	// 选择 bank
	def.Banks = FLASH_BANK_2;
	def.Sector = SectorIndexToDefine(sector_index);
	def.NbSectors = 1;

	// 每次擦除 64 位，直到擦除整个 bank。
	// 越高的电压擦除时并行的位数就越多。
	def.VoltageRange = FLASH_VOLTAGE_RANGE_4;

	/* 擦除到某个扇区的时候如果发生错误，会停止擦除，将扇区索引赋值给 error_sector_index，
	 * 然后返回。
	 */
	HAL_StatusTypeDef result = HAL_FLASHEx_Erase_IT(&def);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动擦除流程失败"};
	}

	_operation_completed.Acquire();
	if (_operation_failed)
	{
		throw std::runtime_error{"擦除流程结束，出错了"};
	}

	SCB_CleanInvalidateDCache();
}

void bsp::Flash::EraseBank_NoIT()
{
	base::UnlockGuard ul{*this};
	FLASH_EraseInitTypeDef def;

	// 擦除类型为 bank 擦除
	def.TypeErase = FLASH_TYPEERASE_MASSERASE;

	// 选择 bank
	def.Banks = FLASH_BANK_2;

	// 每次擦除 64 位，直到擦除整个 bank。
	// 越高的电压擦除时并行的位数就越多。
	def.VoltageRange = FLASH_VOLTAGE_RANGE_4;

	/* 擦除到某个扇区的时候如果发生错误，会停止擦除，将扇区索引赋值给 error_sector_index，
	 * 然后返回。
	 */
	uint32_t error_code;
	HAL_StatusTypeDef result = HAL_FLASHEx_Erase(&def, &error_code);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动擦除流程失败"};
	}

	SCB_CleanInvalidateDCache();
}

void bsp::Flash::EraseSector_NoIT(int32_t sector_index)
{
	base::UnlockGuard ul{*this};
	FLASH_EraseInitTypeDef def;

	// 擦除类型为扇区擦除
	def.TypeErase = FLASH_TYPEERASE_SECTORS;

	// 选择 bank
	def.Banks = FLASH_BANK_2;
	def.Sector = SectorIndexToDefine(sector_index);
	def.NbSectors = 1;

	// 每次擦除 64 位，直到擦除整个 bank。
	// 越高的电压擦除时并行的位数就越多。
	def.VoltageRange = FLASH_VOLTAGE_RANGE_4;

	/* 擦除到某个扇区的时候如果发生错误，会停止擦除，将扇区索引赋值给 error_sector_index，
	 * 然后返回。
	 */
	uint32_t error_code;
	HAL_StatusTypeDef result = HAL_FLASHEx_Erase(&def, &error_code);
	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动擦除流程失败"};
	}

	SCB_CleanInvalidateDCache();
}

#pragma endregion

#pragma region 编程

void bsp::Flash::Program(size_t addr, uint8_t const *buffer)
{
	base::UnlockGuard ul{*this};
	if (addr % ProgrammingSize() != 0)
	{
		throw std::invalid_argument{"addr 必须 32 字节对齐，即要能被 32 整除"};
	}

	if (reinterpret_cast<size_t>(buffer) % 4 != 0)
	{
		throw std::invalid_argument{"buffer 必须 4 字节对齐，即要能被 4 整除"};
	}

	HAL_StatusTypeDef result = HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_FLASHWORD,
													static_cast<uint32_t>(BaseAddress() + addr),
													reinterpret_cast<uint32_t>(buffer));

	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动编程时发生错误"};
	}

	_operation_completed.Acquire();
	if (_operation_failed)
	{
		throw std::runtime_error{"擦除流程结束，出错了"};
	}

	SCB_CleanInvalidateDCache();
}

void bsp::Flash::Program_NoIT(size_t addr, uint8_t const *buffer)
{
	base::UnlockGuard ul{*this};
	if (addr % ProgrammingSize() != 0)
	{
		throw std::invalid_argument{"addr 必须 32 字节对齐，即要能被 32 整除"};
	}

	if (reinterpret_cast<size_t>(buffer) % 4 != 0)
	{
		throw std::invalid_argument{"buffer 必须 4 字节对齐，即要能被 4 整除"};
	}

	HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,
												 static_cast<uint32_t>(BaseAddress() + addr),
												 reinterpret_cast<uint32_t>(buffer));

	if (result != HAL_StatusTypeDef::HAL_OK)
	{
		throw std::runtime_error{"启动编程时发生错误"};
	}

	SCB_CleanInvalidateDCache();
}

#pragma endregion

void bsp::Flash::Read(size_t addr, uint8_t *buffer, int32_t count)
{
	base::UnlockGuard ul{*this};
	uint8_t *absolute_address = reinterpret_cast<uint8_t *>(BaseAddress() + addr);
	std::copy(absolute_address, absolute_address + count, buffer);
}

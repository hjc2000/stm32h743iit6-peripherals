#include "flash_handle.h" // IWYU pragma: keep
#include "base/string/define.h"
#include "RamFlash.h"
#include <stdexcept>

std::shared_ptr<base::flash::flash_handle> base::flash::open(uint32_t id)
{
	switch (id)
	{
	case 1:
		{
			return std::shared_ptr<base::flash::flash_handle>{new bsp::RamFlash{}};
		}
	default:
		{
			throw std::invalid_argument{CODE_POS_STR + "非法 ID."};
		}
	}
}

void base::flash::initialize(base::flash::flash_handle &self)
{
	self.Initialize();
}

int64_t base::flash::sector_size(base::flash::flash_handle &self)
{
	return self.SectorSize();
}

int64_t base::flash::sector_count(base::flash::flash_handle &self)
{
	return self.SectorCount();
}

int64_t base::flash::reading_size(base::flash::flash_handle &self)
{
	return self.ReadingSize();
}

int64_t base::flash::programming_size(base::flash::flash_handle &self)
{
	return self.ProgrammingSize();
}

void base::flash::erase_sector(base::flash::flash_handle &self, int64_t sector_index)
{
	self.EraseSector(sector_index);
}

void base::flash::read_sector(base::flash::flash_handle &self,
							  int64_t sector_index,
							  int64_t offset,
							  base::Span const &span)
{
	self.ReadSector(sector_index, offset, span);
}

void base::flash::program_sector(base::flash::flash_handle &self,
								 int64_t sector_index,
								 int64_t offset,
								 base::ReadOnlySpan const &span)
{
	self.ProgramSector(sector_index, offset, span);
}

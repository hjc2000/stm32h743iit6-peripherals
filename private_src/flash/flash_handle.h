#pragma once
#include "base/embedded/flash/flash_handle.h"

class base::flash::flash_handle
{
public:
	virtual void Initialize() = 0;

	virtual int64_t SectorSize() const = 0;

	virtual int64_t SectorCount() const = 0;

	virtual int64_t ReadingSize() const = 0;

	virtual int64_t ProgrammingSize() const = 0;

	virtual void EraseSector(int64_t sector_index) = 0;

	virtual void ReadSector(int64_t sector_index,
							int64_t offset,
							base::Span const &span) = 0;

	virtual void ProgramSector(int64_t sector_index,
							   int64_t offset,
							   base::ReadOnlySpan const &span) = 0;
};

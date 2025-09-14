#pragma once
#include "base/embedded/flash/RamFlash.h"
#include "flash_handle.h"

namespace bsp
{
	class RamFlash final :
		public base::flash::flash_handle
	{
	private:
		base::flash::RamFlash _ram_flash{};

	public:
		virtual void Initialize() override
		{
			_ram_flash.Initialize();
		}

		virtual int64_t SectorSize() const override
		{
			return _ram_flash.SectorSize();
		}

		virtual int64_t SectorCount() const override
		{
			return _ram_flash.SectorCount();
		}

		virtual int64_t ReadingSize() const override
		{
			return _ram_flash.ReadingSize();
		}

		virtual int64_t ProgrammingSize() const override
		{
			return _ram_flash.ProgrammingSize();
		}

		virtual void EraseSector(int64_t sector_index) override
		{
			_ram_flash.EraseSector(sector_index);
		}

		virtual void ReadSector(int64_t sector_index,
								int64_t offset,
								base::Span const &span) override
		{
			_ram_flash.ReadSector(sector_index, offset, span);
		}

		virtual void ProgramSector(int64_t sector_index,
								   int64_t offset,
								   base::ReadOnlySpan const &span) override
		{
			_ram_flash.ProgramSector(sector_index, offset, span);
		}
	};

} // namespace bsp

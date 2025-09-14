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
			try
			{
				_ram_flash.EraseSector(sector_index);
			}
			catch (std::exception const &e)
			{
				throw std::runtime_error{CODE_POS_STR + e.what()};
			}
			catch (...)
			{
				throw std::runtime_error{CODE_POS_STR + "未知的异常。"};
			}
		}

		virtual void ReadSector(int64_t sector_index,
								int64_t offset,
								base::Span const &span) override
		{
			try
			{
				_ram_flash.ReadSector(sector_index, offset, span);
			}
			catch (std::exception const &e)
			{
				throw std::runtime_error{CODE_POS_STR + e.what()};
			}
			catch (...)
			{
				throw std::runtime_error{CODE_POS_STR + "未知的异常。"};
			}
		}

		virtual void ProgramSector(int64_t sector_index,
								   int64_t offset,
								   base::ReadOnlySpan const &span) override
		{
			try
			{
				_ram_flash.ProgramSector(sector_index, offset, span);
			}
			catch (std::exception const &e)
			{
				throw std::runtime_error{CODE_POS_STR + e.what()};
			}
			catch (...)
			{
				throw std::runtime_error{CODE_POS_STR + "未知的异常。"};
			}
		}
	};

} // namespace bsp

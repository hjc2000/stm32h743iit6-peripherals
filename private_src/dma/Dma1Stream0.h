#pragma once
#include "DmaContext.h"

namespace bsp
{
	namespace dma
	{
		class Dma1Stream0 :
			public base::dma::IDma
		{
		private:
			Dma1Stream0()
			{
				_context._handle.Instance = DMA1_Stream0;
			}

			base::dma::DmaContext _context{};

		public:
			static Dma1Stream0 &Instance();

			virtual base::dma::DmaContext &Context() override
			{
				return _context;
			}
		};

	} // namespace dma
} // namespace bsp

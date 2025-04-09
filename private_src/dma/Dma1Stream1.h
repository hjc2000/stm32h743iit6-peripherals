#pragma once
#include "DmaContext.h"

namespace bsp
{
	namespace dma
	{
		class Dma1Stream1 :
			public base::dma::IDma
		{
		private:
			Dma1Stream1()
			{
				_context._handle.Instance = DMA1_Stream1;
			}

			base::dma::DmaContext _context{};

		public:
			static Dma1Stream1 &Instance();

			virtual base::dma::DmaContext &Context() override
			{
				return _context;
			}
		};

	} // namespace dma
} // namespace bsp

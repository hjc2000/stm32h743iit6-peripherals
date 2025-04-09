#include "Dma1Stream1.h"

PREINIT(bsp::dma::Dma1Stream1::Instance)

bsp::dma::Dma1Stream1 &bsp::dma::Dma1Stream1::Instance()
{
	static Dma1Stream1 o;
	return o;
}

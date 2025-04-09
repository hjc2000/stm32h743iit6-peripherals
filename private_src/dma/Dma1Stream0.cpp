#include "Dma1Stream0.h"

PREINIT(bsp::dma::Dma1Stream0::Instance)

bsp::dma::Dma1Stream0 &bsp::dma::Dma1Stream0::Instance()
{
	static Dma1Stream0 o;
	return o;
}

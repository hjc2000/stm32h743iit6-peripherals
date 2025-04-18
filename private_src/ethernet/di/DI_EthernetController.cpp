#include "ethernet_controller_handle.h"
#include <bsp-interface/di/ethernet.h>

/// @brief 以太网控制器。
/// @return
bsp::IEthernetController &bsp::di::ethernet::EthernetController()
{
	return base::ethernet::ethernet_controller_handle::Instance();
}

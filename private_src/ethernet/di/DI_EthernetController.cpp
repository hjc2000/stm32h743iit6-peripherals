#include <bsp-interface/di/ethernet.h>
#include <EthernetController.h>

/// @brief 以太网控制器。
/// @return
bsp::IEthernetController &bsp::di::ethernet::EthernetController()
{
    return bsp::EthernetController::Instance();
}

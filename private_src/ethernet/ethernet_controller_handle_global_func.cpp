#include "ethernet_controller_handle.h"

std::shared_ptr<base::ethernet::ethernet_controller_handle> base::ethernet::open(uint32_t id)
{
	return std::shared_ptr<base::ethernet::ethernet_controller_handle>{new base::ethernet::ethernet_controller_handle{}};
}

void base::ethernet::initialize(base::ethernet::ethernet_controller_handle &h,
								base::ethernet::InterfaceType type,
								uint32_t phy_address,
								base::Mac const &mac)
{
	h.Open(type, phy_address, mac);
}

base::Mac base::ethernet::mac(base::ethernet::ethernet_controller_handle &h)
{
	return h.Mac();
}

/* #region 控制 PHY */

uint32_t base::ethernet::read_phy_register(base::ethernet::ethernet_controller_handle &h,
										   uint32_t reg_index)
{
	return h.ReadPHYRegister(reg_index);
}

void base::ethernet::write_phy_register(base::ethernet::ethernet_controller_handle &h,
										uint32_t reg_index,
										uint32_t value)
{
	h.WritePHYRegister(reg_index, value);
}

/* #endregion */

///
/// @brief 启动以太网控制器。
///
/// @note 通过读写 PHY 寄存器，控制 PHY 的初始化，等到 PHY 与远程连接后，读取出
/// 使用的双工模式和连接速率，用来启动以太网控制器。因为以太网控制器不支持自动从 PHY
/// 中获取这些信息。
///
/// @param h
/// @param duplex_mode 双工模式。
/// @param speed 连接速率。
///
void base::ethernet::start(base::ethernet::ethernet_controller_handle &h,
						   base::ethernet::DuplexMode duplex_mode,
						   base::Mbps const &speed)
{
	h.Start(duplex_mode, speed);
}

/* #region 收发数据 */

void base::ethernet::send(base::ethernet::ethernet_controller_handle &h,
						  base::ReadOnlySpan const &span)
{
	h.Send(span);
}

void base::ethernet::send(base::ethernet::ethernet_controller_handle &h,
						  std::vector<base::ReadOnlySpan> const &spans)
{
	h.Send(spans);
}

base::ReadOnlySpan base::ethernet::receive(base::ethernet::ethernet_controller_handle &h)
{
	return h.Receive();
}

/* #endregion */

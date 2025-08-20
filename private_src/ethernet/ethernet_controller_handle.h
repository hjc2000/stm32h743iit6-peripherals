#pragma once
#include "base/embedded/ethernet/ethernet_controller_handle.h"

class base::ethernet::ethernet_controller_handle
{
public:
	///
	/// @brief 以太网控制器的名称。
	///
	/// @return
	///
	virtual std::string Name() const = 0;

	///
	/// @brief 获取本控制器绑定的 MAC 地址。
	///
	/// @note 要打开本控制器后本属性才有效。
	///
	/// @return
	///
	virtual base::Mac Mac() const = 0;

	///
	/// @brief 打开以太网控制器。
	///
	/// @param interface_type 连接着 PHY 的接口类型。（使用的是 MII 还是 RMII）
	/// @param phy_address PHY 的地址。
	/// @param mac MAC 地址。
	///
	virtual void Initialize(base::ethernet::InterfaceType interface_type,
							uint32_t phy_address,
							base::Mac const &mac) = 0;

	///
	/// @brief 读 PHY 的寄存器
	///
	/// @param register_index 寄存器索引。
	///
	/// @return
	///
	virtual uint32_t ReadPHYRegister(uint32_t register_index) = 0;

	///
	/// @brief 写 PHY 的寄存器。
	///
	/// @param register_index 寄存器索引。
	/// @param value
	///
	virtual void WritePHYRegister(uint32_t register_index, uint32_t value) = 0;

	///
	/// @brief 启动以太网。
	///
	/// @param duplex_mode
	/// @param speed
	///
	virtual void Start(base::ethernet::DuplexMode duplex_mode,
					   base::unit::Mbps const &speed) = 0;

	///
	/// @brief 发送。
	///
	/// @param spans
	///
	virtual void Send(std::vector<base::ReadOnlySpan> const &spans) = 0;

	///
	/// @brief 发送单个 span.
	///
	/// @note 默认实现是基于 void Send(base::IEnumerable<base::ReadOnlySpan> const &spans).
	///
	/// @param span
	///
	virtual void Send(base::ReadOnlySpan const &span) = 0;

	/// @brief 接收。
	///
	/// @note 因为接收后需要解析，而解析需要数据完整且连续，所以必须接收一整个完整的以太网帧，
	/// 放到一个 span 中。
	///
	/// @return
	///
	virtual base::ReadOnlySpan Receive() = 0;
};

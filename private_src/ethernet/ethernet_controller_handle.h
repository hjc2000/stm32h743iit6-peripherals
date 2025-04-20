#pragma once
#include "base/container/List.h"
#include "base/peripheral/ethernet/ethernet_controller_handle.h"
#include "base/peripheral/ethernet/parameter.h"
#include "base/task/BinarySemaphore.h"
#include "base/unit/Mbps.h"
#include "base/UsageStateManager.h"
#include "hal.h"

class base::ethernet::ethernet_controller_handle
{
private:
	base::UsageStateManager<ethernet_controller_handle> _usage_state_manager;

	/* #region 句柄 */

	class handle_context
	{
	public:
		handle_context(ethernet_controller_handle *self)
			: _self(self)
		{
		}

		ETH_HandleTypeDef _handle{};
		ethernet_controller_handle *_self{};
	};

	handle_context _handle_context{this};
	/* #endregion */

	base::ethernet::InterfaceType _interface_type;
	uint32_t _phy_address = 0;
	base::Mac _mac;
	ETH_TxPacketConfig _sending_config{};
	base::List<ETH_BufferTypeDef> _eth_buffers{};
	base::task::BinarySemaphore _send_completion_signal{false};
	base::List<base::ReadOnlySpan> _received_span_list{};
	base::task::BinarySemaphore _receiving_completion_signal{false};

	/// @brief 接收。
	/// @return
	base::IEnumerable<base::ReadOnlySpan> const &ReceiveMultiSpans();

public:
	ethernet_controller_handle();

	/// @brief 以太网控制器的名称。
	/// @return
	std::string Name() const;

	/// @brief 获取本控制器绑定的 MAC 地址。
	/// @note 要打开本控制器后本属性才有效。
	/// @return
	base::Mac Mac() const;

	/// @brief 打开以太网控制器。
	/// @param interface_type 连接着 PHY 的接口类型。（使用的是 MII 还是 RMII）
	/// @param phy_address PHY 的地址。
	/// @param mac MAC 地址。
	void Open(base::ethernet::InterfaceType interface_type,
			  uint32_t phy_address,
			  base::Mac const &mac);

	/// @brief 读 PHY 的寄存器
	/// @param register_index 寄存器索引。
	/// @return
	uint32_t ReadPHYRegister(uint32_t register_index);

	/// @brief 写 PHY 的寄存器。
	/// @param register_index 寄存器索引。
	/// @param value
	void WritePHYRegister(uint32_t register_index, uint32_t value);

	/// @brief 启动以太网。
	/// @param duplex_mode
	/// @param speed
	void Start(base::ethernet::DuplexMode duplex_mode,
			   base::Mbps const &speed);

	/// @brief 发送。
	/// @param spans
	void Send(std::vector<base::ReadOnlySpan> const &spans);

	/// @brief 发送单个 span.
	/// @note 默认实现是基于 void Send(base::IEnumerable<base::ReadOnlySpan> const &spans).
	/// @param span
	void Send(base::ReadOnlySpan const &span);

	/// @brief 接收。
	/// @note 因为接收后需要解析，而解析需要数据完整且连续，所以必须接收一整个完整的以太网帧，
	/// 放到一个 span 中。
	/// @return
	base::ReadOnlySpan Receive();
};

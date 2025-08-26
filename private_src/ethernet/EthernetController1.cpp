#include "EthernetController1.h"
#include "base/Console.h"
#include "base/embedded/cache/cache.h"
#include "base/embedded/ethernet/ethernet_controller_handle.h"
#include "base/embedded/interrupt/interrupt.h"
#include "bsp-interface/di/interrupt.h"
#include "hal.h"

bsp::EthernetController1::EthernetController1()
{
	base::ethernet::msp_initialize_callback(1);

	// MPU 设置
	{
		MPU_Region_InitTypeDef MPU_InitStruct{};
		HAL_MPU_Disable();
		MPU_InitStruct.Enable = MPU_REGION_ENABLE;
		MPU_InitStruct.BaseAddress = 0x30040000;
		MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
		MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
		MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
		MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
		MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
		MPU_InitStruct.Number = MPU_REGION_NUMBER5;
		MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
		MPU_InitStruct.SubRegionDisable = 0x00;
		MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
		HAL_MPU_ConfigRegion(&MPU_InitStruct);
		HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
	}

	_sending_config.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
	_sending_config.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
	_sending_config.CRCPadCtrl = ETH_CRC_PAD_INSERT;

	_send_completion_signal.Release();
}

base::IEnumerable<base::ReadOnlySpan> const &bsp::EthernetController1::ReceiveMultiSpans()
{
	while (true)
	{
		_received_span_list.Clear();

		ETH_BufferTypeDef rx_buffers[ETH_RX_DESC_CNT]{};
		for (uint32_t i = 0; i < ETH_RX_DESC_CNT - 1; i++)
		{
			rx_buffers[i].next = &rx_buffers[i + 1];
		}

		if (!HAL_ETH_IsRxDataAvailable(&_handle_context._handle))
		{
			// 无数据可接收，等待信号量。有数据到来会触发中断，中断服务函数会释放信号量。
			_receiving_completion_signal.Acquire();
		}

		if (HAL_ETH_GetRxDataBuffer(&_handle_context._handle, rx_buffers) != HAL_OK)
		{
			base::console.WriteLine("HAL_ETH_GetRxDataBuffer 接收数据发生错误。");
			continue;
		}

		HAL_ETH_BuildRxDescriptors(&_handle_context._handle);

		for (ETH_BufferTypeDef buffer : rx_buffers)
		{
			if (buffer.buffer == nullptr)
			{
				break;
			}

			if (buffer.len == 0)
			{
				break;
			}

			base::cache::invalidate_d_cache(buffer.buffer, buffer.len);
			base::ReadOnlySpan span{buffer.buffer, static_cast<int32_t>(buffer.len)};
			_received_span_list.Add(span);
			if (buffer.next == nullptr)
			{
				break;
			}
		}

		if (_received_span_list.Count() > 0)
		{
			return _received_span_list;
		}
	}
}

std::string bsp::EthernetController1::Name() const
{
	return "eth";
}

base::Mac bsp::EthernetController1::Mac() const
{
	return _mac;
}

void bsp::EthernetController1::Initialize(base::ethernet::InterfaceType interface_type,
										  uint32_t phy_address,
										  base::Mac const &mac)
{
	_interface_type = interface_type;
	_phy_address = phy_address;
	_mac = mac;

	uint8_t big_endian_mac_buffer[6];
	base::Span big_endian_mac_buffer_span{big_endian_mac_buffer, sizeof(big_endian_mac_buffer)};
	big_endian_mac_buffer_span.CopyFrom(mac.Span());
	big_endian_mac_buffer_span.Reverse();

	_handle_context._handle.Instance = ETH;
	_handle_context._handle.Init.MACAddr = big_endian_mac_buffer;
	_handle_context._handle.Init.MediaInterface = HAL_ETH_RMII_MODE;
	_handle_context._handle.Init.RxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(0x30040000);
	_handle_context._handle.Init.TxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(0x30040000 + 4 * sizeof(ETH_DMADescTypeDef));
	_handle_context._handle.Init.RxBuffLen = ETH_MAX_PACKET_SIZE;
	HAL_StatusTypeDef result = HAL_ETH_Init(&_handle_context._handle);
	if (result != HAL_OK)
	{
		throw std::runtime_error{"打开以太网口失败"};
	}

	_handle_context._handle.TxCpltCallback = [](ETH_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->_send_completion_signal.ReleaseFromIsr();
	};

	_handle_context._handle.RxCpltCallback = [](ETH_HandleTypeDef *handle)
	{
		handle_context *context = reinterpret_cast<handle_context *>(handle);
		context->_self->_receiving_completion_signal.ReleaseFromIsr();
	};

	// MDC时钟
	HAL_ETH_SetMDIOClockRange(&_handle_context._handle);

	using buffer_type = uint8_t (*)[ETH_MAX_PACKET_SIZE];
	buffer_type buffer = reinterpret_cast<buffer_type>(0x30040200);

	for (int idx = 0; idx < static_cast<int>(ETH_RX_DESC_CNT); idx++)
	{
		HAL_ETH_DescAssignMemory(&_handle_context._handle,
								 idx,
								 buffer[idx],
								 NULL);
	}

	// 中断优先级必须设置在 freertos 能够屏蔽的优先级范围内。
	// freertos 无法屏蔽的中断中禁止使用 freertos 的 API.
	base::interrupt::enable_interrupt(static_cast<uint32_t>(ETH_IRQn), 5);

	bsp::di::interrupt::IsrManager().AddIsr(static_cast<uint32_t>(ETH_IRQn),
											[&]()
											{
												HAL_ETH_IRQHandler(&_handle_context._handle);
											});
}

uint32_t bsp::EthernetController1::ReadPHYRegister(uint32_t register_index)
{
	uint32_t regval = 0;
	HAL_StatusTypeDef result = HAL_ETH_ReadPHYRegister(&_handle_context._handle,
													   _phy_address,
													   register_index,
													   &regval);

	if (result != HAL_StatusTypeDef ::HAL_OK)
	{
		throw std::runtime_error{"读 PHY 寄存器失败"};
	}

	return regval;
}

void bsp::EthernetController1::WritePHYRegister(uint32_t register_index, uint32_t value)
{
	HAL_StatusTypeDef result = HAL_ETH_WritePHYRegister(&_handle_context._handle,
														_phy_address,
														register_index,
														value);

	if (result != HAL_StatusTypeDef ::HAL_OK)
	{
		throw std::runtime_error{"写 PHY 寄存器失败"};
	}
}

void bsp::EthernetController1::Start(base::ethernet::DuplexMode duplex_mode, base::unit::Mbps const &speed)
{
	{
		ETH_MACConfigTypeDef def{};
		HAL_ETH_GetMACConfig(&_handle_context._handle, &def);

		if (speed == base::unit::Mbps{10})
		{
			def.Speed = ETH_SPEED_10M;
		}
		else
		{
			def.Speed = ETH_SPEED_100M;
		}

		if (duplex_mode == base::ethernet::DuplexMode::HalfDuplex)
		{
			def.DuplexMode = ETH_HALFDUPLEX_MODE;
		}
		else
		{
			def.DuplexMode = ETH_FULLDUPLEX_MODE;
		}

		HAL_ETH_SetMACConfig(&_handle_context._handle, &def);
	}

	// 设置过滤规则
	{
		ETH_MACFilterConfigTypeDef def{};
		HAL_ETH_GetMACFilterConfig(&_handle_context._handle, &def);
		def.PromiscuousMode = FunctionalState::ENABLE;
		// def.PassAllMulticast = FunctionalState::ENABLE;
		HAL_ETH_SetMACFilterConfig(&_handle_context._handle, &def);
	}

	HAL_ETH_Start_IT(&_handle_context._handle);
}

void bsp::EthernetController1::Send(std::vector<base::ReadOnlySpan> const &spans)
{
	_send_completion_signal.Acquire();
	_sending_config.Length = 0;
	_eth_buffers.Clear();
	for (auto span : spans)
	{
		_sending_config.Length += span.Size();

		ETH_BufferTypeDef eth_buffer{};
		eth_buffer.buffer = const_cast<uint8_t *>(span.Buffer());
		eth_buffer.len = span.Size();

		_eth_buffers.Add(eth_buffer);
		if (_eth_buffers.Count() > 1)
		{
			_eth_buffers[_eth_buffers.Count() - 2].next = &_eth_buffers[_eth_buffers.Count() - 1];
		}
	}

	if (_eth_buffers.Count() > 0)
	{
		_sending_config.TxBuffer = &_eth_buffers[0];
		HAL_ETH_Transmit_IT(&_handle_context._handle, &_sending_config);
	}
}

void bsp::EthernetController1::Send(base::ReadOnlySpan const &span)
{
	_send_completion_signal.Acquire();
	_sending_config.Length = span.Size();

	ETH_BufferTypeDef eth_buffer{};
	eth_buffer.buffer = const_cast<uint8_t *>(span.Buffer());
	eth_buffer.len = span.Size();

	_sending_config.TxBuffer = &eth_buffer;
	HAL_ETH_Transmit_IT(&_handle_context._handle, &_sending_config);
}

base::ReadOnlySpan bsp::EthernetController1::Receive()
{
	while (true)
	{
		_received_span_list.Clear();

		ETH_BufferTypeDef rx_buffers[ETH_RX_DESC_CNT]{};
		for (uint32_t i = 0; i < ETH_RX_DESC_CNT - 1; i++)
		{
			rx_buffers[i].next = &rx_buffers[i + 1];
		}

		if (!HAL_ETH_IsRxDataAvailable(&_handle_context._handle))
		{
			// 无数据可接收，等待信号量。有数据到来会触发中断，中断服务函数会释放信号量。
			_receiving_completion_signal.Acquire();
		}

		if (HAL_ETH_GetRxDataBuffer(&_handle_context._handle, rx_buffers) != HAL_OK)
		{
			base::console.WriteLine("HAL_ETH_GetRxDataBuffer 接收数据发生错误。");
			continue;
		}

		HAL_ETH_BuildRxDescriptors(&_handle_context._handle);

		for (ETH_BufferTypeDef buffer : rx_buffers)
		{
			if (buffer.buffer == nullptr)
			{
				break;
			}

			if (buffer.len == 0)
			{
				break;
			}

			base::cache::invalidate_d_cache(buffer.buffer, buffer.len);
			base::ReadOnlySpan span{buffer.buffer, static_cast<int32_t>(buffer.len)};
			_received_span_list.Add(span);
			if (buffer.next == nullptr)
			{
				break;
			}
		}

		if (_received_span_list.Count() > 1)
		{
			base::console.WriteLine("接收到的以太网帧使用了超过 1 个 DMA 描述符。");
		}

		if (_received_span_list.Count() == 0)
		{
			continue;
		}

		return _received_span_list[0];
	}
}

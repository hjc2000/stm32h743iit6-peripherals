#pragma once
#include "base/embedded/usb/fs-device-pcd/usb_fs_pcd_handle.h"
#include "base/string/define.h"
#include "hal.h" // IWYU pragma: keep
#include <cstdint>
#include <stdexcept>

namespace bsp
{
	namespace usb::fs_device_pcd
	{
		constexpr uint32_t phy_type_to_define_value(base::usb::fs_device_pcd::PhyType phy_type)
		{
			switch (phy_type)
			{
			case base::usb::fs_device_pcd::PhyType::ULPI:
				{
					return PCD_PHY_ULPI;
				}
			case base::usb::fs_device_pcd::PhyType::UTMI:
				{
					return PCD_PHY_UTMI;
				}
			case base::usb::fs_device_pcd::PhyType::Embedded:
				{
					return PCD_PHY_EMBEDDED;
				}
			default:
				{
					throw std::invalid_argument{CODE_POS_STR + "非法 PHY 类型。"};
				}
			}
		}

	} // namespace usb::fs_device_pcd
} // namespace bsp

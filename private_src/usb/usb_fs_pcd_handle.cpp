#include "usb_fs_pcd_handle.h"
#include "UsbFsPcd.h"
#include <memory>

std::shared_ptr<base::usb::fs_pcd::usb_fs_pcd_handle> base::usb::fs_pcd::open(uint32_t id)
{
	return std::shared_ptr<base::usb::fs_pcd::usb_fs_pcd_handle>{new bsp::UsbFsPcd{}};
}

void base::usb::fs_pcd::initialize_as_device(base::usb::fs_pcd::usb_fs_pcd_handle &self,
											 base::usb::PhyType phy_type)
{
	self.InitializeAsDevice(phy_type);
}

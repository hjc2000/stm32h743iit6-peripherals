#include "usb_fs_pcd_handle.h"

std::shared_ptr<base::usb_fs_pcd::usb_fs_pcd_handle> base::usb_fs_pcd::open(uint32_t id);

void base::usb_fs_pcd::initialize_as_device(base::usb_fs_pcd::usb_fs_pcd_handle &self,
											std::string const &clock_source_name,
											uint32_t divider,
											base::usb_fs_pcd::PhyType phy_type)
{
	self.InitializeAsDevice(clock_source_name,
							divider,
							phy_type);
}

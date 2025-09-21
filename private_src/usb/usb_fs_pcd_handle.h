#pragma once
#include "base/embedded/usb/usb_fs_pcd_handle.h"

class base::usb_fs_pcd::usb_fs_pcd_handle
{
public:
	virtual void InitializeAsDevice(std::string const &clock_source_name,
									uint32_t divider,
									base::usb_fs_pcd::PhyType phy_type) = 0;
};

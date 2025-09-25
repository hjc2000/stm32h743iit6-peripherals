#pragma once
#include "base/embedded/usb/usb_fs_pcd_handle.h"

class base::usb::fs_pcd::usb_fs_pcd_handle
{
public:
	virtual void InitializeAsDevice(base::usb::PhyType phy_type) = 0;
};

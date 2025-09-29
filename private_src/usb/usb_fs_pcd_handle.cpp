#include "usb_fs_pcd_handle.h"
#include "UsbFsPcd.h"
#include <memory>

std::shared_ptr<base::usb::fs_device_pcd::usb_fs_pcd_handle> base::usb::fs_device_pcd::open(uint32_t id)
{
	return std::shared_ptr<base::usb::fs_device_pcd::usb_fs_pcd_handle>{new bsp::UsbFsPcd{}};
}

void base::usb::fs_device_pcd::initialize_as_device(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
													base::usb::PhyType phy_type)
{
	self.InitializeAsDevice(phy_type);
}

void base::usb::fs_device_pcd::start(base::usb::fs_device_pcd::usb_fs_pcd_handle &self)
{
	self.Start();
}

void base::usb::fs_device_pcd::suspend(base::usb::fs_device_pcd::usb_fs_pcd_handle &self)
{
	self.Suspend();
}

void base::usb::fs_device_pcd::resume(base::usb::fs_device_pcd::usb_fs_pcd_handle &self)
{
	self.Resume();
}

/* #region 注册回调 */

void base::usb::fs_device_pcd::set_sof_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
												std::function<void()> const &callback)
{
	self.SetSofCallback(callback);
}

void base::usb::fs_device_pcd::set_setup_stage_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
														std::function<void(base::usb::fs_device_pcd::SetupStageCallbackArgs const &)> const &callback)
{
	self.SetSetupStageCallback(callback);
}

void base::usb::fs_device_pcd::set_reset_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
												  std::function<void()> const &callback)
{
	self.SetResetCallback(callback);
}

void base::usb::fs_device_pcd::set_suspend_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
													std::function<void()> const &callback)
{
	self.SetSuspendCallback(callback);
}

void base::usb::fs_device_pcd::set_resume_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
												   std::function<void()> const &callback)
{
	self.SetResumeCallback(callback);
}

void base::usb::fs_device_pcd::set_connect_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
													std::function<void()> const &callback)
{
	self.SetConnectCallback(callback);
}

void base::usb::fs_device_pcd::set_disconnect_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
													   std::function<void()> const &callback)
{
	self.SetDisconnectCallback(callback);
}

void base::usb::fs_device_pcd::set_data_out_stage_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
														   std::function<void(base::usb::fs_device_pcd::DataOutStageCallbackArgs const &)> const &callback)
{
	self.SetDataOutStageCallback(callback);
}

void base::usb::fs_device_pcd::set_data_in_stage_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
														  std::function<void(base::usb::fs_device_pcd::DataInStageCallbackArgs const &)> const &callback)
{
	self.SetDataInStageCallback(self, callback);
}

void base::usb::fs_device_pcd::set_iso_out_incomplete_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
															   std::function<void(base::usb::fs_device_pcd::IsoOutIncompleteCallbackArgs const &)> const &callback)
{
	self.SetIsoOutIncompleteCallback(callback);
}

void base::usb::fs_device_pcd::set_iso_in_incomplete_callback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
															  std::function<void(base::usb::fs_device_pcd::IsoInIncompleteCallbackArgs const &)> const &callback)
{
	self.SetIsoInIncompleteCallback(callback);
}

/* #endregion */

#pragma once
#include "base/embedded/usb/fs_device_pcd/usb_fs_pcd_handle.h"

class base::usb::fs_device_pcd::usb_fs_pcd_handle
{
public:
	virtual void InitializeAsDevice(base::usb::PhyType phy_type) = 0;

	virtual void Start() = 0;

	virtual void Suspend() = 0;

	virtual void Resume() = 0;

	/* #region 设置回调函数 */

	virtual void SetSofCallback(std::function<void()> const &callback) = 0;

	virtual void SetSetupStageCallback(std::function<void(base::usb::fs_device_pcd::SetupStageCallbackArgs const &)> const &callback) = 0;

	virtual void SetResetCallback(std::function<void()> const &callback) = 0;

	virtual void SetSuspendCallback(std::function<void()> const &callback) = 0;

	virtual void SetResumeCallback(std::function<void()> const &callback) = 0;

	virtual void SetConnectCallback(std::function<void()> const &callback) = 0;

	virtual void SetDisconnectCallback(std::function<void()> const &callback) = 0;

	virtual void SetDataOutStageCallback(std::function<void(base::usb::fs_device_pcd::DataOutStageCallbackArgs const &)> const &callback) = 0;

	virtual void SetDataInStageCallback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
										std::function<void(base::usb::fs_device_pcd::DataInStageCallbackArgs const &)> const &callback) = 0;

	/* #endregion */
};

#pragma once
#include "base/embedded/usb/usb_fs_pcd_handle.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "usb_fs_pcd_handle.h"
#include <cstdint>

namespace bsp
{
	class UsbFsPcd :
		public base::usb_fs_pcd::usb_fs_pcd_handle
	{
	private:
		class handle_context
		{
		public:
			handle_context(UsbFsPcd *self)
				: _self{self}
			{
			}

			PCD_HandleTypeDef _handle{};
			UsbFsPcd *_self{};
		};

		base::UsageStateManager<UsbFsPcd> _usage_state_manager{};
		handle_context _handle_context{this};

		void InitializeCallback();

		void OnSofCallback()
		{
		}

		void OnSetupStageCallback()
		{
		}

		void OnResetCallback()
		{
		}

		void OnSuspendCallback()
		{
		}

		void OnResumeCallback()
		{
		}

		void OnConnectCallback()
		{
		}

		void OnDisconnectCallback()
		{
		}

		void OnDataOutStageCallback(uint8_t epnum)
		{
		}

		void OnDataInStageCallback(uint8_t epnum)
		{
		}

		void OnISOOUTIncompleteCallback(uint8_t epnum)
		{
		}

		void OnISOINIncompleteCallback(uint8_t epnum)
		{
		}

	public:
		UsbFsPcd()
		{
			base::usb_fs_pcd::msp_initialize(1);
		}

		virtual void InitializeAsDevice(std::string const &clock_source_name,
										uint32_t divider,
										base::usb_fs_pcd::PhyType phy_type) override;
	};

} // namespace bsp

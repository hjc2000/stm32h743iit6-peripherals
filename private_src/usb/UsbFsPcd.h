#pragma once
#include "base/embedded/interrupt/interrupt.h"
#include "base/embedded/usb/fs-device-pcd/usb_fs_pcd_handle.h"
#include "base/stream/ReadOnlySpan.h"
#include "base/string/define.h"
#include "base/UsageStateManager.h"
#include "hal.h" // IWYU pragma: keep
#include "usb_fs_pcd_handle.h"
#include <cstdint>
#include <functional>
#include <stdexcept>

namespace bsp
{
	class UsbFsPcd final :
		public base::usb::fs_device_pcd::usb_fs_pcd_handle
	{
	private:
		class hal_pcd_handle_context
		{
		public:
			hal_pcd_handle_context(UsbFsPcd *self)
				: _self{self}
			{
			}

			PCD_HandleTypeDef _handle{};
			UsbFsPcd *_self{};
		};

		base::UsageStateManager<UsbFsPcd> _usage_state_manager{};
		hal_pcd_handle_context _handle_context{this};
		inline static PCD_HandleTypeDef *_handle{};

		void InitializeInterrupt();

		void InitializeCallback();

		/* #region USB 回调 */

		std::function<void()> _sof_callback;
		std::function<void(base::usb::fs_device_pcd::SetupStageCallbackArgs const &)> _setup_stage_callback;
		std::function<void()> _reset_callback;
		std::function<void()> _suspend_callback;
		std::function<void()> _resume_callback;
		std::function<void()> _connect_callback;
		std::function<void()> _disconnect_callback;
		std::function<void(base::usb::fs_device_pcd::DataOutStageCallbackArgs const &)> _data_out_stage_callback;
		std::function<void(base::usb::fs_device_pcd::DataInStageCallbackArgs const &)> _data_in_stage_callback;
		std::function<void(base::usb::fs_device_pcd::IsoOutIncompleteCallbackArgs const &)> _iso_out_incomplete_callback;
		std::function<void(base::usb::fs_device_pcd::IsoInIncompleteCallbackArgs const &)> _iso_in_incomplete_callback;

		void OnSofCallback()
		{
			if (_sof_callback)
			{
				_sof_callback();
			}
		}

		void OnSetupStageCallback()
		{
			if (_setup_stage_callback)
			{
				base::ReadOnlySpan span{
					reinterpret_cast<uint8_t const *>(_handle_context._handle.Setup),
					sizeof(_handle_context._handle.Setup),
				};

				base::usb::fs_device_pcd::SetupStageCallbackArgs args{span};

				_setup_stage_callback(args);
			}
		}

		void OnResetCallback()
		{
			if (_reset_callback)
			{
				_reset_callback();
			}
		}

		void OnSuspendCallback()
		{
			if (_suspend_callback)
			{
				_suspend_callback();
			}
		}

		void OnResumeCallback()
		{
			if (_resume_callback)
			{
				_resume_callback();
			}
		}

		void OnConnectCallback()
		{
			if (_connect_callback)
			{
				_connect_callback();
			}
		}

		void OnDisconnectCallback()
		{
			if (_disconnect_callback)
			{
				_disconnect_callback();
			}
		}

		void OnDataOutStageCallback(uint8_t epnum)
		{
			if (_data_out_stage_callback)
			{
				base::ReadOnlySpan span{
					_handle_context._handle.OUT_ep[epnum].xfer_buff,
					HAL_PCD_EP_GetRxCount(&_handle_context._handle, epnum),
				};

				base::usb::fs_device_pcd::DataOutStageCallbackArgs args{epnum, span};
				_data_out_stage_callback(args);
			}
		}

		void OnDataInStageCallback(uint8_t epnum)
		{
			if (_data_in_stage_callback)
			{
				base::usb::fs_device_pcd::DataInStageCallbackArgs args{epnum};
				_data_in_stage_callback(args);
			}
		}

		void OnIsoOutIncompleteCallback(uint8_t epnum)
		{
			if (_iso_out_incomplete_callback)
			{
				base::usb::fs_device_pcd::IsoOutIncompleteCallbackArgs args{epnum};
				_iso_out_incomplete_callback(args);
			}
		}

		void OnIsoInIncompleteCallback(uint8_t epnum)
		{
			if (_iso_in_incomplete_callback)
			{
				base::usb::fs_device_pcd::IsoInIncompleteCallbackArgs args{epnum};
				_iso_in_incomplete_callback(args);
			}
		}

		/* #endregion */

	public:
		UsbFsPcd()
		{
			base::usb::fs_device_pcd::msp_initialize(1);
			_handle = &_handle_context._handle;
		}

		virtual void Initialize(base::usb::PhyType phy_type) override;

		virtual void Start() override
		{
			HAL_StatusTypeDef result = HAL_PCD_Start(&_handle_context._handle);
			if (result != HAL_StatusTypeDef::HAL_OK)
			{
				throw std::runtime_error{CODE_POS_STR + "启动失败。"};
			}

			base::interrupt::enable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn), 5);
		}

		virtual void Suspend() override
		{
			__HAL_PCD_GATE_PHYCLOCK(&_handle_context._handle);

			if (_handle_context._handle.Init.low_power_enable)
			{
				/* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register. */
				SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
			}
		}

		virtual void Resume() override
		{
		}

		static PCD_HandleTypeDef &HalPcdHandle()
		{
			return *_handle;
		}

		/* #region 注册回调 */

		virtual void SetSofCallback(std::function<void()> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_sof_callback = callback;
		}

		virtual void SetSetupStageCallback(std::function<void(base::usb::fs_device_pcd::SetupStageCallbackArgs const &)> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_setup_stage_callback = callback;
		}

		virtual void SetResetCallback(std::function<void()> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_reset_callback = callback;
		}

		virtual void SetSuspendCallback(std::function<void()> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_suspend_callback = callback;
		}

		virtual void SetResumeCallback(std::function<void()> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_resume_callback = callback;
		}

		virtual void SetConnectCallback(std::function<void()> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_connect_callback = callback;
		}

		virtual void SetDisconnectCallback(std::function<void()> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_disconnect_callback = callback;
		}

		virtual void SetDataOutStageCallback(std::function<void(base::usb::fs_device_pcd::DataOutStageCallbackArgs const &)> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_data_out_stage_callback = callback;
		}

		virtual void SetDataInStageCallback(base::usb::fs_device_pcd::usb_fs_pcd_handle &self,
											std::function<void(base::usb::fs_device_pcd::DataInStageCallbackArgs const &)> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_data_in_stage_callback = callback;
		}

		virtual void SetIsoOutIncompleteCallback(std::function<void(base::usb::fs_device_pcd::IsoOutIncompleteCallbackArgs const &)> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_iso_out_incomplete_callback = callback;
		}

		virtual void SetIsoInIncompleteCallback(std::function<void(base::usb::fs_device_pcd::IsoInIncompleteCallbackArgs const &)> const &callback) override
		{
			base::interrupt::disable_interrupt(static_cast<int32_t>(IRQn_Type::OTG_FS_IRQn));
			_iso_in_incomplete_callback = callback;
		}

		/* #endregion */
	};

} // namespace bsp

#pragma once
#include "bsp-interface/clock/IClockSource.h"
#include "clock_source_handle.h"

namespace bsp
{
	/// @brief hse 时钟源。需要外部晶振。
	class HseClockSource :
		public base::clock::clock_source_handle
	{
	private:
		base::MHz _frequency{};
		IClockSource_State _state{};

	public:
		virtual base::MHz Frequency() const override;

		virtual void Configure() override;

		virtual void Configure(std::map<uint32_t, uint32_t> const &channel_factor_map) override;

		/// @brief 打开时钟源。
		/// @note 有的时钟源非常简单，外部振荡器输入后，直接就输出同频率的时钟信号，
		/// 不支持分频和倍频。本重载用于这种简单的时钟源。
		/// @param crystal_frequency
		void Open(base::MHz const &crystal_frequency);

		/// @brief 设置为旁路。
		/// @param external_clock_frequency 外部时钟频率。
		void SetAsBypass(base::MHz external_clock_frequency);

		/// @brief 关闭时钟源。
		void Close();

		/// @brief 本时钟源当前的状态。
		/// @return
		IClockSource_State State() const;
	};
} // namespace bsp

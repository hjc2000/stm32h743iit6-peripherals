#pragma once
#include "base/UsageStateManager.h"
#include "independent_watch_dog_handle.h"

namespace bsp
{
	class IndependentWatchDog1 final :
		public base::independent_watch_dog::independent_watch_dog_handle
	{
	private:
		base::UsageStateManager<IndependentWatchDog1> _usage_state_manager{};
		std::chrono::milliseconds _timeout{};

	public:
		///
		/// @brief 初始化独立看门狗。
		///
		/// @param timeout
		///
		virtual void Initialize(std::chrono::milliseconds const &timeout) override
		{
		}

		///
		/// @brief 喂狗。
		///
		virtual void Feed() override
		{
		}

		///
		/// @brief 看门狗超时时间。
		///
		/// @return
		///
		virtual std::chrono::milliseconds Timeout() override
		{
			return _timeout;
		}
	};

} // namespace bsp

#pragma once
#include "base/embedded/watch-dog/independent_watch_dog_handle.h"

class base::independent_watch_dog::independent_watch_dog_handle
{
public:
	virtual ~independent_watch_dog_handle() = default;

	///
	/// @brief 初始化独立看门狗。
	///
	/// @param timeout
	///
	virtual void Initialize(std::chrono::milliseconds const &timeout) = 0;

	///
	/// @brief 喂狗。
	///
	virtual void Feed() = 0;

	///
	/// @brief 看门狗超时时间。
	///
	/// @return
	///
	virtual std::chrono::milliseconds Timeout() = 0;
};

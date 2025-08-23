#pragma once
#include "base/embedded/timer/base_timer_handle.h"

class base::base_timer::base_timer_handle
{
public:
	///
	/// @brief 初始化定时器。
	///
	/// @param period
	///
	virtual void Initialize(std::chrono::nanoseconds const &period) = 0;

	///
	/// @brief 定时时间。
	///
	/// @return
	///
	virtual std::chrono::nanoseconds Period() = 0;

	///
	/// @brief 设置定时时间。
	///
	/// @note 允许运行时实时修改定时时间。
	///
	/// @note 此函数要实现为能够在定时时间到的回调中被调用，因此实现者需要避免会出现
	/// 死锁的实现方式。
	///
	/// @param period
	///
	virtual void SetPeriod(std::chrono::nanoseconds const &period) = 0;

	///
	/// @brief 设置定时时间到的回调。
	///
	/// @note 单片机平台直接实现为在中断中回调，不要太早使用线程和信号量进行封装。
	/// 到时候在这个基础上设计一个包装类，包装直接使用中断进行回调的定时器，使用信号量
	/// 释放被阻塞的任务去执行定时任务。
	///
	/// @warning 实现者实现时需要注意在修改回调函数的时候禁用定时器中断。
	///
	/// @param callback
	///
	virtual void SetPeriodElapsedCallback(std::function<void()> const &callback) = 0;

	///
	/// @brief 启动定时器。
	///
	virtual void Start() = 0;

	///
	/// @brief 停止定时器。
	///
	virtual void Stop() = 0;
};

#include "base/peripheral/systick/systick.h"
#include "base/define.h"
#include "base/task/delay.h"
#include "base/unit/Hz.h"
#include "base/unit/Seconds.h"
#include "bsp-interface/di/interrupt.h"
#include "bsp-interface/timer/ISysTick.h"

///
/// @brief 获取当前频率。
///
/// @return
///
base::MHz base::systick::frequency();

///
/// @brief 获取当前的计数值。
///
/// @note systick 被认为是减计数的。如果实际上是增计数的，可以将本函数实现为返回计数最大值减去
/// 当前值，这样就虚拟成减计数的了。
///
/// @return
///
uint64_t base::systick::current_value();

///
/// @brief 计数器的重装载值。
///
/// @note 计数器减计数到 0 后，下一个时钟周期会重新变成 reload_value 的值。
///
/// @return
///
uint64_t base::systick::reload_value();

///
/// @brief 定时时间到处理函数。
///
/// @warning 实现者需要将本函数实现为线程安全和可重入。
/// 	@note 最简单的方法就是在本函数的操作期间禁用全局中断。
/// 	@note 最佳实践是只禁用 systick 的中断。
///
/// @warning 不要在中断函数中调用本函数。永远只能在主程序中调用本函数。
///
/// @param func
///
void base::systick::set_elapsed_handler(std::function<void()> func);

///
/// @brief 系统时间戳。
///
/// @return
///
std::chrono::nanoseconds base::systick::system_time_stamp();

#include "independent_watch_dog_handle.h" // IWYU pragma: keep

///
/// @brief 打开独立看门狗。
///
/// @param id
/// @return
///
std::shared_ptr<base::independent_watch_dog::independent_watch_dog_handle> base::independent_watch_dog::open(uint32_t id);

///
/// @brief 初始化独立看门狗。
///
/// @param self
/// @param timeout
///
void base::independent_watch_dog::initialize(base::independent_watch_dog::independent_watch_dog_handle &self,
											 std::chrono::milliseconds const &timeout)
{
	self.Initialize(timeout);
}

///
/// @brief 喂狗。
///
/// @param self
///
void base::independent_watch_dog::feed(base::independent_watch_dog::independent_watch_dog_handle &self)
{
	self.Feed();
}

///
/// @brief 看门狗超时时间。
///
/// @param self
/// @return
///
std::chrono::milliseconds base::independent_watch_dog::timeout(base::independent_watch_dog::independent_watch_dog_handle &self)
{
	return self.Timeout();
}

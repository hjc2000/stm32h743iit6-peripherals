#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
	class GpioPinPE4 final :
		public bsp::GpioPin
	{
	private:
		GpioPinPE4() = default;

	public:
		static_function GpioPinPE4 &Instance();

		GPIO_TypeDef *Port() override;
		uint32_t Pin() override;

		/// @brief 引脚名称
		/// @return
		std::string PinName() const override
		{
			return "PE4";
		}
	};
} // namespace bsp

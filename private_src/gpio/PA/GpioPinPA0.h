#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
    class GpioPinPA0 final :
        public bsp::GpioPin
    {
    private:
        GpioPinPA0() = default;

        base::Array<std::string, 1> _supported_alternate_functions{
            "af_push_pull",
        };

    public:
        static_function GpioPinPA0 &Instance()
        {
            class Getter :
                public bsp::TaskSingletonGetter<GpioPinPA0>
            {
            public:
                std::unique_ptr<GpioPinPA0> Create() override
                {
                    return std::unique_ptr<GpioPinPA0>{new GpioPinPA0{}};
                }
            };

            Getter o;
            return o.Instance();
        }

        GPIO_TypeDef *Port() override;
        uint32_t Pin() override;

        /// @brief 引脚名称
        /// @return
        std::string PinName() const override;

        /// @brief 支持的复用功能。
        /// @note 复用功能是非常复杂的，各个型号单片机都不一样，所以采用字符串。
        /// @note 假设某个单片机的一个引脚的复用功能有：uart1, timer1_compare_output 等。
        /// 这种名称是与具体型号高度相关的，所以本库无法提供一个枚举来列举这些情况。
        /// @return
        base::IEnumerable<std::string> &SupportedAlternateFunctions() override
        {
            return _supported_alternate_functions;
        }
    };
} // namespace bsp

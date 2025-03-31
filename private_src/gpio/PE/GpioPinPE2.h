#pragma once
#include <base/container/Array.h>
#include <base/RentedPtrFactory.h>
#include <GpioPin.h>

namespace bsp
{
    class GpioPinPE2 final :
        public bsp::GpioPin
    {
    private:
        GpioPinPE2() = default;

        base::Array<std::string, 1> _supported_alternate_functions{"gpio"};

    public:
        static_function GpioPinPE2 &Instance()
        {
            class Getter :
                public bsp::TaskSingletonGetter<GpioPinPE2>
            {
            public:
                std::unique_ptr<GpioPinPE2> Create() override
                {
                    return std::unique_ptr<GpioPinPE2>{new GpioPinPE2{}};
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

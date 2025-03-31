#include "GpioPinPF15.h"
#include <hal.h>

bsp::GpioPinPF15 &bsp::GpioPinPF15::Instance()
{
    class Getter :
        public bsp::TaskSingletonGetter<GpioPinPF15>
    {
    public:
        std::unique_ptr<GpioPinPF15> Create() override
        {
            return std::unique_ptr<GpioPinPF15>{new GpioPinPF15{}};
        }
    };

    Getter o;
    return o.Instance();
}

GPIO_TypeDef *bsp::GpioPinPF15::Port()
{
    return GPIOF;
}

uint32_t bsp::GpioPinPF15::Pin()
{
    return GPIO_PIN_15;
}

std::string bsp::GpioPinPF15::PinName() const
{
    return "PF15";
}

base::IEnumerable<std::string> &bsp::GpioPinPF15::SupportedAlternateFunctions()
{
    return _supported_alternate_functions;
}

void bsp::GpioPinPF15::OpenAsAlternateFunctionMode(std::string function_name, bsp::IGpioPinPullMode pull_mode, bsp::IGpioPinDriver driver_mode)
{
    if (_is_open)
    {
        throw std::runtime_error{PinName() + " 已经打开"};
    }

    EnableClock();
    GPIO_InitTypeDef def{};
    if (function_name == "fmc")
    {
        def.Alternate = GPIO_AF12_FMC;
    }
    else
    {
        throw std::invalid_argument{"不支持的 AlternateFunction"};
    }

    switch (pull_mode)
    {
    default:
    case bsp::IGpioPinPullMode::NoPull:
        {
            def.Pull = GPIO_NOPULL;
            break;
        }
    case bsp::IGpioPinPullMode::PullUp:
        {
            def.Pull = GPIO_PULLUP;
            break;
        }
    case bsp::IGpioPinPullMode::PullDown:
        {
            def.Pull = GPIO_PULLDOWN;
            break;
        }
    }

    switch (driver_mode)
    {
    case bsp::IGpioPinDriver::PushPull:
        {
            def.Mode = GPIO_MODE_AF_PP;
            break;
        }
    case bsp::IGpioPinDriver::OpenDrain:
        {
            def.Mode = GPIO_MODE_AF_OD;
            break;
        }
    default:
        {
            throw std::invalid_argument{"不支持的 Driver"};
        }
    }

    def.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    def.Pin = Pin();
    HAL_GPIO_Init(Port(), &def);
    _is_open = true;
}

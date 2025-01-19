#include <base/container/Dictionary.h>
#include <bsp-interface/di/serial.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <Serial.h>

/// @brief 包含所有可用串口的集合。
/// @return
base::IDictionary<std::string, bsp::serial::ISerial *> const &DI_SerialCollection()
{
    class Collection
    {
    private:
        Collection()
        {
            Add(&bsp::Serial::Instance());
        }

        void Add(bsp::serial::ISerial *o)
        {
            _dic.Add(o->Name(), o);
        }

    public:
        base::Dictionary<std::string, bsp::serial::ISerial *> _dic{};

        static_function Collection &Instance()
        {
            class Getter :
                public bsp::TaskSingletonGetter<Collection>
            {
            public:
                std::unique_ptr<Collection> Create() override
                {
                    return std::unique_ptr<Collection>{new Collection{}};
                }
            };

            Getter g;
            return g.Instance();
        }
    };

    return Collection::Instance()._dic;
}

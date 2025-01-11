#include <base/container/Dictionary.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/serial.h>
#include <bsp-interface/di/task.h>
#include <Serial.h>

/// @brief 包含所有可用串口的集合。
/// @return
base::IDictionary<std::string, bsp::ISerial *> const &DI_SerialCollection()
{
    class Collection
    {
    private:
        Collection()
        {
            Add(&bsp::Serial::Instance());
        }

        void Add(bsp::ISerial *o)
        {
            _dic.Add(o->Name(), o);
        }

    public:
        base::Dictionary<std::string, bsp::ISerial *> _dic{};

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

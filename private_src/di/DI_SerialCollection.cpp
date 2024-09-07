#include <base/container/Collection.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/serial.h>
#include <Serial.h>

/// @brief 包含所有可用串口的集合。
/// @return
base::ICollection<std::string, bsp::ISerial *> const &DI_SerialCollection()
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
            _collection.Put(o->Name(), o);
        }

    public:
        base::Collection<std::string, bsp::ISerial *> _collection{};

        static Collection &Instance()
        {
            class Getter : public base::SingletonGetter<Collection>
            {
            public:
                std::unique_ptr<Collection> Create() override
                {
                    return std::unique_ptr<Collection>{new Collection{}};
                }

                void Lock() override
                {
                    DI_InterruptSwitch().DisableGlobalInterrupt();
                }

                void Unlock() override
                {
                    DI_InterruptSwitch().EnableGlobalInterrupt();
                }
            };

            Getter g;
            return g.Instance();
        }
    };

    return Collection::Instance()._collection;
}

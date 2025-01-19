#include <base/container/Dictionary.h>
#include <bsp-interface/di/serial.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <Serial.h>

namespace
{
    class Collection
    {
    private:
        void Add(bsp::serial::ISerial *o)
        {
            _dic.Add(o->Name(), o);
        }

    public:
        Collection()
        {
            Add(&bsp::Serial::Instance());
        }

        base::Dictionary<std::string, bsp::serial::ISerial *> _dic{};
    };

    class Getter :
        public bsp::TaskSingletonGetter<Collection>
    {
    public:
        std::unique_ptr<Collection> Create() override
        {
            return std::unique_ptr<Collection>{new Collection{}};
        }
    };
} // namespace

/// @brief 包含所有可用串口的集合。
/// @return
base::IDictionary<std::string, bsp::serial::ISerial *> const &bsp::di::serial::SerialCollection()
{
    Getter g;
    return g.Instance()._dic;
}

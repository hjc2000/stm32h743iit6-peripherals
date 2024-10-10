#pragma once
#include <base/define.h>
#include <base/di/SingletonGetter.h>
#include <bsp-interface/di/dma.h>
#include <bsp-interface/di/interrupt.h>
#include <bsp-interface/di/task.h>
#include <bsp-interface/serial/ISerial.h>
#include <SerialOptions.h>

namespace bsp
{
    class Serial :
        public bsp::ISerial
    {
    private:
        Serial() = default;

        bool _have_begun = false;
        UART_HandleTypeDef _uart_handle{};
        std::shared_ptr<bsp::IBinarySemaphore> _sending_completion_signal = DICreate_BinarySemaphore();
        std::shared_ptr<bsp::IBinarySemaphore> _receiving_completion_signal = DICreate_BinarySemaphore();
        std::shared_ptr<bsp::IMutex> _read_lock = DICreate_Mutex();

        bsp::IDmaChannel *_rx_dma_channel = nullptr;
        bsp::IDmaChannel *_tx_dma_channel = nullptr;

#pragma region 初始化
        void InitializeGpio();
        void InitializeDma();
        void InitializeUart(SerialOptions const &options);
        void InitializeInterrupt();
#pragma endregion

        /// @brief 通过串口句柄和 DMA 寄存器，获取当前 DMA 接收了多少个字节。
        /// @return
        int32_t HaveRead();

#pragma region 被中断处理函数回调的函数
        static_function void OnReceiveEventCallback(UART_HandleTypeDef *huart, uint16_t pos);
        static_function void OnSendCompleteCallback(UART_HandleTypeDef *huart);
        static_function void OnReadTimeout(UART_HandleTypeDef *huart);
#pragma endregion

        void SetReadTimeoutByBaudCount(uint32_t value);

    public:
        static_function Serial &Instance();

        std::string Name() override
        {
            return "serial";
        }

#pragma region Stream
        /// @brief 调用后临时启动 DMA 接收一次数据。
        /// @note 本类没有缓冲机制，所以上层应用如果调用 Read 不及时，会丢失数据。
        /// @note 因为调用一次 Read 具有一定开销，需要设置寄存器，使能中断，设置一些
        /// 状态变量。所以为了提高效率，每次调用 Read 时传入的 buffer 适当大一些，
        /// 并且 count 大一些。
        ///
        /// @param buffer
        /// @param offset
        /// @param count
        /// @return
        int32_t Read(uint8_t *buffer, int32_t offset, int32_t count) override;

        /// @brief 调用后临时启动 DMA 进行一次发送。
        /// @param buffer
        /// @param offset
        /// @param count
        void Write(uint8_t const *buffer, int32_t offset, int32_t count) override;

        void Close() override;
#pragma endregion

        /// @brief 打开串口
        /// @param options
        void Open(bsp::ISerialOptions const &options) override;
    };
} // namespace bsp

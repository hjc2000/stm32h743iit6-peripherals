#pragma once
#include <base/define.h>
#include <bsp-interface/di/sdram.h>
#include <bsp-interface/TaskSingletonGetter.h>
#include <hal.h>

namespace bsp
{
    /// @brief 封装 FMC 接口。
    class SDRAMController :
        public bsp::sdram::ISDRAMController
    {
    private:
        SDRAMController() = default;

        SDRAM_HandleTypeDef _handle{};
        std::shared_ptr<bsp::sdram::ISDRAMTiming> _timing;

        uint8_t mpu_set_protection(uint32_t baseaddr,
                                   uint32_t size,
                                   uint32_t rnum,
                                   uint8_t de,
                                   uint8_t ap,
                                   uint8_t sen,
                                   uint8_t cen,
                                   uint8_t ben)
        {
            MPU_Region_InitTypeDef mpu_region_init_handle{};

            HAL_MPU_Disable(); /* 配置MPU之前先关闭MPU,配置完成以后在使能MPU */

            mpu_region_init_handle.Enable = MPU_REGION_ENABLE;     /* 使能该保护区域 */
            mpu_region_init_handle.Number = rnum;                  /* 设置保护区域 */
            mpu_region_init_handle.BaseAddress = baseaddr;         /* 设置基址 */
            mpu_region_init_handle.DisableExec = de;               /* 是否允许指令访问 */
            mpu_region_init_handle.Size = size;                    /* 设置保护区域大小 */
            mpu_region_init_handle.SubRegionDisable = 0X00;        /* 禁止子区域 */
            mpu_region_init_handle.TypeExtField = MPU_TEX_LEVEL0;  /* 设置类型扩展域为level0 */
            mpu_region_init_handle.AccessPermission = (uint8_t)ap; /* 设置访问权限, */
            mpu_region_init_handle.IsShareable = sen;              /* 是否共用? */
            mpu_region_init_handle.IsCacheable = cen;              /* 是否cache? */
            mpu_region_init_handle.IsBufferable = ben;             /* 是否缓冲? */
            HAL_MPU_ConfigRegion(&mpu_region_init_handle);         /* 配置MPU */
            HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);                /* 开启MPU */
            return 0;
        }

        void InitializeGPIO();

        /// @brief 开始自动发送 “自动刷新” 命令。
        /// @param timing
        void StartAutoSendingAutoRefreshCommand(bsp::sdram::ISDRAMTiming const &timing);

    public:
        static_function SDRAMController &Instance();

        /// @brief 将 SDRAM 控制器以读突发的模式打开。写不突发。
        /// @param timing_provider
        /// @param bank_count
        /// @param row_bit_count
        /// @param column_bit_count
        /// @param data_width
        /// @param read_burst_length
        virtual void OpenAsReadBurstMode(bsp::sdram::ISDRAMTimingProvider const &timing_provider,
                                         bsp::sdram::property::BankCount const &bank_count,
                                         bsp::sdram::property::RowBitCount const &row_bit_count,
                                         bsp::sdram::property::ColumnBitCount const &column_bit_count,
                                         bsp::sdram::property::DataWidth const &data_width,
                                         bsp::sdram::property::ReadBurstLength const &read_burst_length) override;

        /// @brief 将输入信号置于空操作命令状态，然后开始向 SDRAM 提供 CLK 信号。
        void PowerUp();

        /// @brief 发送：“预充电所有 BANK” 的命令。
        virtual void PrechargeAll() override;

        /// @brief 发送自动刷新命令。
        virtual void AutoRefresh() override;

        /// @brief 写模式寄存器。
        /// @param value
        virtual void WriteModeRegister(uint32_t value) override;

        /// @brief 控制器被打开后所使用的时序。
        /// @return
        virtual bsp::sdram::ISDRAMTiming const &Timing() const override;

        /// @brief 此 SDRAM 控制器所管理的内存段的起始地址。打开 SDRAM 后，对着这个地址开始往后的内存区域
        /// 读写数据即可读写 SDRAM 的内容。
        /// @return
        virtual uint8_t *StartAddress() const override
        {
            return reinterpret_cast<uint8_t *>(0xC0000000);
        }
    };
} // namespace bsp

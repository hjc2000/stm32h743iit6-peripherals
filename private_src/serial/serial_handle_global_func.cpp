#include "serial_handle.h" // IWYU pragma: keep

///
/// @brief 通过串口 ID 打开串口。
///
/// @param serial_id 串口 ID.
/// 	@note 单片机中使用这种方式。例如想要打开 UART1 就传入 1.
///
/// @return base::serial::sp_serial_handle
///
base::serial::sp_serial_handle open(int serial_id);

///
/// @brief 启动串口。
///
/// @param h
/// @param direction
/// @param baud_rate
/// @param data_bits
/// @param parity
/// @param stop_bits
/// @param hardware_flow_control
///
void start(base::serial::sp_serial_handle const &h,
		   base::serial::Direction direction,
		   base::serial::BaudRate const &baud_rate,
		   base::serial::DataBits const &data_bits,
		   base::serial::Parity parity,
		   base::serial::StopBits stop_bits,
		   base::serial::HardwareFlowControl hardware_flow_control);

/* #region 串口属性 */

///
/// @brief 获取串口名称。
///
/// @param h
/// @return std::string
///
std::string name(base::serial::sp_serial_handle const &h);

///
/// @brief 数据传输方向。
///
/// @param h
/// @return base::serial::Direction
///
base::serial::Direction direction(base::serial::sp_serial_handle const &h);

///
/// @brief 波特率。
///
/// @param h
/// @return uint32_t
///
uint32_t baud_rate(base::serial::sp_serial_handle const &h);

///
/// @brief 数据位的个数。
///
/// @param h
/// @return uint8_t
///
uint8_t data_bits(base::serial::sp_serial_handle const &h);

///
/// @brief 校验位。
///
/// @param h
/// @return base::serial::Parity
///
base::serial::Parity parity(base::serial::sp_serial_handle const &h);

///
/// @brief 停止位个数。
///
/// @param h
/// @return base::serial::StopBits
///
base::serial::StopBits stop_bits(base::serial::sp_serial_handle const &h);

///
/// @brief 硬件流控。
///
/// @param h
/// @return base::serial::HardwareFlowControl
///
base::serial::HardwareFlowControl hardware_flow_control(base::serial::sp_serial_handle const &h);

///
/// @brief 检查串口能否读。
///
/// @param h
/// @return true
/// @return false
///
bool can_read(base::serial::sp_serial_handle const &h);

///
/// @brief 检查串口能否写。
///
/// @param h
/// @return true
/// @return false
///
bool can_write(base::serial::sp_serial_handle const &h);

/* #endregion */

///
/// @brief 从串口读取数据
///
/// @param h
/// @param span
///
/// @return int32_t 成功读取的字节数。永远不应该返回 0. 应该将本函数实现为等同 Stream
/// 的 Read 方法。
///
int32_t read(base::serial::sp_serial_handle const &h, base::Span const &span);

///
/// @brief 向串口写入数据。
///
/// @param h
/// @param span
///
void write(base::serial::sp_serial_handle const &h, base::ReadOnlySpan const &span);

///
/// @brief 冲洗串口。
///
/// @param h
///
void flush(base::serial::sp_serial_handle const &h);

#ifndef __CUS__MSP432_UART__
#define __CUS__MSP432_UART__

//定义
#define MSP432_UART_RECEIVE_BUFFER_LENGTH 200

//抽象类库IO流包含
#include "Abs_IO_Stream.hpp"
//驱动库包含
extern "C"
{
#include "driverlib.h"
}
//标准整形
#include "stdint.h"
namespace cus
{
    // MSP432 串口通讯类实现
    class MSP432_Uart : public Abs_OStream, public Abs_IStream
    {
    private:
        //缓冲区及数据计数器
        char buffer[MSP432_UART_RECEIVE_BUFFER_LENGTH];
        uint16_t buffer_counter;
        //用于scanf的缓冲区头指针
        char *lp_Buffer_Head;

        //是否初正确构造
        bool isInit_already;
        //串口模块记录
        uint32_t EUSCI_Ax_BASE;

        //缓冲区头指针滚动
        void lpBuffer_Head_roll();

    public:
        //默认构造
        MSP432_Uart();

        /**
         * 带参构造函数
         * @param EUSCI_Ax_BASE 要开启串口的外设模块,有如下选项
         * @param -A0 RX_P1.2 TX_P1.3
         * @param -A1 RX_P2.2 TX_P2.3
         * @param -A2 RX_P3.2 TX_P3.3
         * @param -A3 RX_P9.6 TX_P9.7
         * @param Baud_Rate 设置的波特率
         * @arg -仅支持115200和9600
         */
        MSP432_Uart(uint32_t EUSCI_Ax_BASE, uint32_t Baud_Rate);

        /**
         * 构造&类初始化
         * @param EUSCI_Ax_BASE 要开启串口的外设模块,有如下选项
         * @param -A0 RX_P1.2 TX_P1.3
         * @param -A1 RX_P2.2 TX_P2.3
         * @param -A2 RX_P3.2 TX_P3.3
         * @param -A3 RX_P9.6 TX_P9.7
         * @param Baud_Rate 设置的波特率
         * @arg -仅支持115200和9600
         */
        MSP432_Uart &construct(uint32_t EUSCI_Ax_BASE, uint32_t Baud_Rate);

        /**
         *   输出一个字节数据
         *   @param chr 需要输出的字节数据
         *   @return IO_Stream_Error异常抛出
         */
        virtual IO_Stream_Error putchar(const char chr);

        /**
         * 获取一个字节数据
         * @return 获取的字节数据
         */
        virtual char getchar();

        /**
         *   格式化输入
         *   @param lpFormatString 格式化输入字符串
         *   @param ... 要读取的内容参数，支持如下:
         *   @param -%d 读取整数
         *   @param -%f 读取单精度浮点数
         *   @param -%s 读取字符串
         *   @param -%c 读取字符
         *   @param -%xx 标准输入输出支持的其他标签
         *   @return IO_Stream_Error异常抛出
         */
        virtual IO_Stream_Error scanf(const char *lpFormatString, ...);

        /**
         * 开启结束字符中断
         * @param End_Char 定义的结束字符
         * @param lpEUSCI_Ax_End_CallBack 自定义的中断回调函数指针
         * @arg 必须是void 返回值，参数为void的函数
         */
        void enable_EndChar_Interrupt(const char End_Char, void (*lpEUSCI_Ax_End_CallBack)(void));

        //清理缓冲区
        void flush();

        /**
         * 设置波特率
         * @param Baud_Rate 要设置的波特率
         * @arg -仅支持115200和9600
         */
        void set_Baud_Rate(uint32_t Baud_Rate);
    };
}
#endif

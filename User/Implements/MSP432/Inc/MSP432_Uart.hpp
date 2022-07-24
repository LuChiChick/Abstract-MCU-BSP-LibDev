#ifndef __CUS__MSP432_UART__
#define __CUS__MSP432_UART__
//抽象类库IO流包含
#include "Abs_IO_Stream.hpp"
//驱动库包含
#include "driverlib.h"
//标准整形
#include "stdint.h"
namespace cus
{
    // MSP432 串口通讯类实现
    class MSP432_Uart : public Abs_OStream
    {
        //是否初正确构造
        bool isInit_already;

        // UART模块初始化结构体
        eUSCI_UART_ConfigV1 uart_config;

        //串口模块记录
        uint32_t EUSCI_Ax_BASE;

    public:
        //默认构造
        MSP432_Uart();

        /**
         * 带参构造函数
         * @param EUSCI_Ax_BASE 要开启串口的外设模块
         * @param Baud_Rate 设置的波特率
         */
        MSP432_Uart(uint32_t EUSCI_Ax_BASE, uint32_t Baud_Rate);

        /**
         *   输出一个字节数据
         *   @param chr 需要输出的字节数据
         *   @return IO_Stream_Error异常抛出
         */
        virtual IO_Stream_Error putchar(const char chr);

        /**
         * 设置波特率
         * @param Baud_Rate 要设置的波特率
         */
        void set_Baud_Rate(uint32_t Baud_Rate);
    };
}
#endif

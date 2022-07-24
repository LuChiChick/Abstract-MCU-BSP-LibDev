//抽象类库IO流包含
#include "MSP432_Uart.hpp"

namespace cus
{
    //默认构造函数
    MSP432_Uart::MSP432_Uart()
    {
        isInit_already = false;
    }

    /**
     * 带参构造函数
     * @param EUSCI_Ax_BASE 要开启串口的外设模块
     */
    MSP432_Uart::MSP432_Uart(uint32_t EUSCI_Ax_BASE, uint32_t Baud_Rate)
    {
        //初始化EUSCIA模块串口
        uart_config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;        //选择时钟源
        uart_config.parity = EUSCI_A_UART_NO_PARITY;                           //无校验模式
        uart_config.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;                    //低位在前模式
        uart_config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;              //一位停止位模式
        uart_config.uartMode = EUSCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE; //自动猜测波特率模式
        uart_config.dataLength = EUSCI_A_UART_8_BIT_LEN;                       // 8位数据模式

        //要筛选时钟晶振频率信号决定配置结构体参数，经TI官方计算工具计算，目标波特率115200
        {
            //时钟主频48Mhz
            if (SystemCoreClock == CS_48MHZ)
            {
                if (Baud_Rate == 115200)
                {
                    uart_config.clockPrescalar = 26; //预分频器
                    uart_config.firstModReg = 0;     //第一模式标志位
                    uart_config.secondModReg = 111;  //第二模式标志位
                    uart_config.overSampling = 1;    //过采样波特率发生器
                }

                else if (Baud_Rate == 9600)
                {
                    uart_config.clockPrescalar = 312; //预分频器
                    uart_config.firstModReg = 8;      //第一模式标志位
                    uart_config.secondModReg = 0;     //第二模式标志位
                    uart_config.overSampling = 1;     //过采样波特率发生器
                }
            }

            //时钟主频24MHZ
            else if (SystemCoreClock == CS_24MHZ)
            {
                if (Baud_Rate == 115200)
                {
                    uart_config.clockPrescalar = 13; //预分频器
                    uart_config.firstModReg = 0;     //第一模式标志位
                    uart_config.secondModReg = 37;   //第二模式标志位
                    uart_config.overSampling = 1;    //过采样波特率发生器
                }

                else if (Baud_Rate == 9600)
                {
                    uart_config.clockPrescalar = 156; //预分频器
                    uart_config.firstModReg = 4;      //第一模式标志位
                    uart_config.secondModReg = 0;     //第二模式标志位
                    uart_config.overSampling = 1;     //过采样波特率发生器
                }
                else
                {
                    isInit_already = false;
                    return;
                }
            }

            //时钟主频12MHZ
            else if (SystemCoreClock == CS_12MHZ)
            {
                if (Baud_Rate == 115200)
                {
                    uart_config.clockPrescalar = 6; //预分频器
                    uart_config.firstModReg = 8;    //第一模式标志位
                    uart_config.secondModReg = 32;  //第二模式标志位
                    uart_config.overSampling = 1;   //过采样波特率发生器
                }

                else if (Baud_Rate == 9600)
                {
                    uart_config.clockPrescalar = 78; //预分频器
                    uart_config.firstModReg = 2;     //第一模式标志位
                    uart_config.secondModReg = 0;    //第二模式标志位
                    uart_config.overSampling = 1;    //过采样波特率发生器
                }
                else
                {
                    isInit_already = false;
                    return;
                }
            }

            //时钟主频3MHZ
            else if (SystemCoreClock == CS_3MHZ)
            {
                if (Baud_Rate == 115200)
                {
                    uart_config.clockPrescalar = 1; //预分频器
                    uart_config.firstModReg = 10;   //第一模式标志位
                    uart_config.secondModReg = 0;   //第二模式标志位
                    uart_config.overSampling = 1;   //过采样波特率发生器
                }

                else if (Baud_Rate == 9600)
                {
                    uart_config.clockPrescalar = 19; //预分频器
                    uart_config.firstModReg = 8;     //第一模式标志位
                    uart_config.secondModReg = 85;   //第二模式标志位
                    uart_config.overSampling = 1;    //过采样波特率发生器
                }
                else
                {
                    isInit_already = false;
                    return;
                }
            }

            //时钟主频1.5MHZ
            else if (SystemCoreClock == CS_15MHZ)
            {
                if (Baud_Rate == 115200)
                {
                    uart_config.clockPrescalar = 13; //预分频器
                    uart_config.firstModReg = 0;     //第一模式标志位
                    uart_config.secondModReg = 0;    //第二模式标志位
                    uart_config.overSampling = 0;    //过采样波特率发生器
                }

                else if (Baud_Rate == 9600)
                {
                    uart_config.clockPrescalar = 9; //预分频器
                    uart_config.firstModReg = 12;   //第一模式标志位
                    uart_config.secondModReg = 34;  //第二模式标志位
                    uart_config.overSampling = 1;   //过采样波特率发生器
                }
                else
                {
                    isInit_already = false;
                    return;
                }
            }
        }

        //筛查需要配置的EUSCI模块
        switch (EUSCI_Ax_BASE)
        {
        case EUSCI_A0_BASE:
            //开启IO模块第二功能
            GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
            //初始化串口
            UART_initModule(EUSCI_Ax_BASE, &uart_config);
            //开启串口
            UART_enableModule(EUSCI_Ax_BASE);
            //信息记录完成初始化
            this->EUSCI_Ax_BASE = EUSCI_Ax_BASE;
            isInit_already = true;
            break;
        case EUSCI_A1_BASE:
            //开启IO模块第二功能
            GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
            //初始化串口
            UART_initModule(EUSCI_Ax_BASE, &uart_config);
            //开启串口
            UART_enableModule(EUSCI_Ax_BASE);
            //信息记录完成初始化
            this->EUSCI_Ax_BASE = EUSCI_Ax_BASE;
            isInit_already = true;
            break;
        case EUSCI_A2_BASE:
            //开启IO模块第二功能
            GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
            //初始化串口
            UART_initModule(EUSCI_Ax_BASE, &uart_config);
            //开启串口
            UART_enableModule(EUSCI_Ax_BASE);
            //信息记录完成初始化
            this->EUSCI_Ax_BASE = EUSCI_Ax_BASE;
            isInit_already = true;
            break;
        case EUSCI_A3_BASE:
            //开启IO模块第二功能
            GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
            //初始化串口
            UART_initModule(EUSCI_Ax_BASE, &uart_config);
            //开启串口
            UART_enableModule(EUSCI_Ax_BASE);
            //信息记录完成初始化
            this->EUSCI_Ax_BASE = EUSCI_Ax_BASE;
            isInit_already = true;
            break;
        default:
            isInit_already = false;
        }
    }

    /**
     *   输出一个字节数据
     *   @param chr 需要输出的字节数据
     *   @return IO_Stream_Error异常抛出
     */
    IO_Stream_Error MSP432_Uart::putchar(const char chr)
    {
        //检查初始化审查
        if (isInit_already)
        {
            UART_transmitData(EUSCI_Ax_BASE, chr);
            return IO_STREAM_ERROR_NONE;
        }
        else
            return IO_STREAM_ERROR_PUTCHAR_FAILED;
    };
}
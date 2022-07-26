//抽象类库IO流包含
#include "MSP432_Uart.hpp"
#include "string.h"
#include "stdarg.h"

// Uart缓冲区
char *EUSCI_A0_Buffer = NULL;
char *EUSCI_A1_Buffer = NULL;
char *EUSCI_A2_Buffer = NULL;
char *EUSCI_A3_Buffer = NULL;

// Uart缓冲区数据计数
uint16_t *EUSCI_A0_Buffer_Counter;
uint16_t *EUSCI_A1_Buffer_Counter;
uint16_t *EUSCI_A2_Buffer_Counter;
uint16_t *EUSCI_A3_Buffer_Counter;

//帧尾回调定义
char EUSCI_A0_End_Char = '\n';
char EUSCI_A1_End_Char = '\n';
char EUSCI_A2_End_Char = '\n';
char EUSCI_A3_End_Char = '\n';

//帧尾回调函数指针
void (*lpEUSCI_A0_End_CallBack)(void) = NULL;
void (*lpEUSCI_A1_End_CallBack)(void) = NULL;
void (*lpEUSCI_A2_End_CallBack)(void) = NULL;
void (*lpEUSCI_A3_End_CallBack)(void) = NULL;

// EUSCIA0模块接收中断
extern "C" void EUSCIA0_IRQHandler(void)
{
    if (UART_getEnabledInterruptStatus(EUSCI_A0_BASE) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        //查看缓冲区是否存在
        if (EUSCI_A0_Buffer != NULL)
        {
            //带上\0小于缓冲区长度时
            if ((*EUSCI_A0_Buffer_Counter) + 1 < MSP432_UART_RECEIVE_BUFFER_LENGTH)
            {
                //保存字符
                EUSCI_A0_Buffer[*EUSCI_A0_Buffer_Counter] = UART_receiveData(EUSCI_A0_BASE);
                //递增字节计数
                (*EUSCI_A0_Buffer_Counter)++;
                //处理结束中断回调
                if ((EUSCI_A0_Buffer[(*EUSCI_A0_Buffer_Counter) - 1] == EUSCI_A0_End_Char) && (lpEUSCI_A0_End_CallBack != NULL))
                    lpEUSCI_A0_End_CallBack();
            }
            //缓冲区溢出时仅处理结束中断
            else if ((UART_receiveData(EUSCI_A0_BASE) == EUSCI_A0_End_Char) && (lpEUSCI_A0_End_CallBack != NULL))
                lpEUSCI_A0_End_CallBack();
        }
    }
}

// EUSCIA1模块接收中断
extern "C" void EUSCIA1_IRQHandler(void)
{
    if (UART_getEnabledInterruptStatus(EUSCI_A1_BASE) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        //查看缓冲区是否存在
        if (EUSCI_A1_Buffer != NULL)
        {
            //带上\0小于缓冲区长度时
            if ((*EUSCI_A1_Buffer_Counter) + 1 < MSP432_UART_RECEIVE_BUFFER_LENGTH)
            {
                //保存字符
                EUSCI_A1_Buffer[*EUSCI_A1_Buffer_Counter] = UART_receiveData(EUSCI_A1_BASE);
                //递增字节计数
                (*EUSCI_A1_Buffer_Counter)++;
                //处理结束中断回调
                if ((EUSCI_A1_Buffer[*EUSCI_A1_Buffer_Counter] == EUSCI_A1_End_Char) && (lpEUSCI_A1_End_CallBack != NULL))
                    lpEUSCI_A1_End_CallBack();
            }
            //缓冲区溢出时仅处理结束中断
            else if ((UART_receiveData(EUSCI_A1_BASE) == EUSCI_A1_End_Char) && (lpEUSCI_A1_End_CallBack != NULL))
                lpEUSCI_A1_End_CallBack();
        }
    }
}

// EUSCIA2模块接收中断
extern "C" void EUSCIA2_IRQHandler(void)
{
    if (UART_getEnabledInterruptStatus(EUSCI_A2_BASE) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        //查看缓冲区是否存在
        if (EUSCI_A2_Buffer != NULL)
        {
            //带上\0小于缓冲区长度时
            if ((*EUSCI_A2_Buffer_Counter) + 1 < MSP432_UART_RECEIVE_BUFFER_LENGTH)
            {
                //保存字符
                EUSCI_A2_Buffer[*EUSCI_A2_Buffer_Counter] = UART_receiveData(EUSCI_A2_BASE);
                //递增字节计数
                (*EUSCI_A2_Buffer_Counter)++;
                //处理结束中断回调
                if ((EUSCI_A2_Buffer[*EUSCI_A2_Buffer_Counter] == EUSCI_A2_End_Char) && (lpEUSCI_A2_End_CallBack != NULL))
                    lpEUSCI_A2_End_CallBack();
            }
            //缓冲区溢出时仅处理结束中断
            else if ((UART_receiveData(EUSCI_A2_BASE) == EUSCI_A2_End_Char) && (lpEUSCI_A2_End_CallBack != NULL))
                lpEUSCI_A2_End_CallBack();
        }
    }
}

// EUSCIA3模块接收中断
extern "C" void EUSCIA3_IRQHandler(void)
{
    if (UART_getEnabledInterruptStatus(EUSCI_A3_BASE) & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        //查看缓冲区是否存在
        if (EUSCI_A3_Buffer != NULL)
        {
            //带上\0小于缓冲区长度时
            if ((*EUSCI_A3_Buffer_Counter) + 1 < MSP432_UART_RECEIVE_BUFFER_LENGTH)
            {
                //保存字符
                EUSCI_A3_Buffer[*EUSCI_A3_Buffer_Counter] = UART_receiveData(EUSCI_A3_BASE);
                //递增字节计数
                (*EUSCI_A3_Buffer_Counter)++;
                //处理结束中断回调
                if ((EUSCI_A3_Buffer[*EUSCI_A3_Buffer_Counter] == EUSCI_A3_End_Char) && (lpEUSCI_A3_End_CallBack != NULL))
                    lpEUSCI_A3_End_CallBack();
            }
            //缓冲区溢出时仅处理结束中断
            else if ((UART_receiveData(EUSCI_A3_BASE) == EUSCI_A3_End_Char) && (lpEUSCI_A3_End_CallBack != NULL))
                lpEUSCI_A3_End_CallBack();
        }
    }
}

namespace cus
{
    //默认构造函数
    MSP432_Uart::MSP432_Uart()
    {
        isInit_already = false;
    }

    /**
     * 带参构造函数
     * @param EUSCI_Ax_BASE 要开启串口的外设模块,有如下选项
     * @param -A0 RX_P1.2 TX_P1.3
     * @param -A1 RX_P2.2 TX_P2.3
     * @param -A2 RX_P3.2 TX_P3.3
     * @param -A3 RX_P9.6 TX_P9.7
     * @param Baud_Rate 设置的波特率
     */
    MSP432_Uart::MSP432_Uart(uint32_t EUSCI_Ax_BASE, uint32_t Baud_Rate)
    {
        construct(EUSCI_Ax_BASE, Baud_Rate);
    }

    /**
     * 构造&类初始化
     * @param EUSCI_Ax_BASE 要开启串口的外设模块,有如下选项
     * @param -A0 RX_P1.2 TX_P1.3
     * @param -A1 RX_P2.2 TX_P2.3
     * @param -A2 RX_P3.2 TX_P3.3
     * @param -A3 RX_P9.6 TX_P9.7
     * @param Baud_Rate 设置的波特率
     */
    MSP432_Uart &MSP432_Uart::construct(uint32_t EUSCI_Ax_BASE, uint32_t Baud_Rate)
    {
        // UART模块初始化结构体
        eUSCI_UART_ConfigV1 uart_config;
        //初始化EUSCIA模块串口
        uart_config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;        //选择时钟源
        uart_config.parity = EUSCI_A_UART_NO_PARITY;                           //无校验模式
        uart_config.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;                    //低位在前模式
        uart_config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;              //一位停止位模式
        uart_config.uartMode = EUSCI_A_UART_AUTOMATIC_BAUDRATE_DETECTION_MODE; //串口波特率自动匹配模式
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
                    return *this;
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
                    return *this;
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
                    return *this;
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
                    return *this;
                }
            }
        }

        //检查是否正确模块
        if (EUSCI_Ax_BASE == EUSCI_A0_BASE || EUSCI_Ax_BASE == EUSCI_A1_BASE || EUSCI_Ax_BASE == EUSCI_A2_BASE || EUSCI_Ax_BASE == EUSCI_A3_BASE)
        {
            //筛查需要配置的EUSCI模块
            switch (EUSCI_Ax_BASE)
            {
            case EUSCI_A0_BASE:
                //开启IO模块第二功能
                GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
                GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
                EUSCI_A0_Buffer = buffer;
                EUSCI_A0_Buffer_Counter = &buffer_counter;
                //开启串口端口中断
                Interrupt_enableInterrupt(INT_EUSCIA0);
                break;
            case EUSCI_A1_BASE:
                //开启IO模块第二功能
                GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
                GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
                //开启串口端口中断
                Interrupt_enableInterrupt(INT_EUSCIA1);
                break;
            case EUSCI_A2_BASE:
                //开启IO模块第二功能
                GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
                GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
                //开启串口端口中断
                Interrupt_enableInterrupt(INT_EUSCIA2);
                break;
            case EUSCI_A3_BASE:
                //开启IO模块第二功能
                GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P9, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
                GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
                //开启串口端口中断
                Interrupt_enableInterrupt(INT_EUSCIA3);
                break;
            }

            //清理缓冲区
            flush();

            //初始化串口
            UART_initModule(EUSCI_Ax_BASE, &uart_config);
            //开启串口
            UART_enableModule(EUSCI_Ax_BASE);
            //开启串口中断
            UART_enableInterrupt(EUSCI_Ax_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
            //开启总中断
            Interrupt_enableMaster();
            //信息记录完成初始化
            this->EUSCI_Ax_BASE = EUSCI_Ax_BASE;
            isInit_already = true;
        }
        else
        {
            isInit_already = false;
        }

        return *this;
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
            //检查启动文件是否分配堆内存，EUSCI中断接收的东西是放在EUSCI模块的仅有的两字节缓存的
            //如果连续写入，他会把多的东西放在堆内存，如果没有足够堆内存，会抱死标志位
            // UART_transmitData内部定义会陷入死循环
            UART_transmitData(EUSCI_Ax_BASE, chr);
            return IO_STREAM_ERROR_NONE;
        }
        else
            return IO_STREAM_ERROR_UNINITED;
    };

    /**
     * 获取一个字节数据
     * @return 获取的字节数据
     */
    char MSP432_Uart::getchar()
    {
        char chr;
        this->scanf("%c", &chr);
        return chr;
    }

    //缓冲区头滚动
    void MSP432_Uart::lpBuffer_Head_roll()
    {
        while (1)
        {
            //检测到消息已经处理完
            if (lp_Buffer_Head[0] == '\0')
            {
                flush();
                break;
            }
            else if ((lp_Buffer_Head[0] == ' ') || (lp_Buffer_Head[0] == '\n'))
            {
                lp_Buffer_Head++;
                if ((lp_Buffer_Head[0] == ' ') || (lp_Buffer_Head[0] == '\n'))
                    continue;
                else
                    break;
            }
            //有其作用，用于筛查其他字符诸如'\r'
            lp_Buffer_Head++;
        }
    }

    /**
     *   格式化输入
     *   @param lpFormatString 格式化输入字符串
     *   @param ... 要读取的内容参数，支持如下:
     *   @param -%d 读取整数
     *   @param -%f 读取单精度浮点数
     *   @param -%lf 读取双精度浮点数
     *   @param -%s 读取字符串
     *   @param -%c 读取字符
     *   @return IO_Stream_Error异常抛出
     */
    IO_Stream_Error MSP432_Uart::scanf(const char *lpFormatString, ...)
    {
        //检查初始化
        if (isInit_already)
        {
            //初始化参数列表，指向不定参第一个参数
            va_list args;
            va_start(args, lpFormatString);

            //探查读取标签
            while (lpFormatString[0] != '\0')
            {
                //检查是否读取结束
                if (lp_Buffer_Head[0] != '\0')
                {
                    if (lpFormatString[0] == '%')
                    {
                        switch (lpFormatString[1])
                        {
                        case 's':
                        {
                            char *lpString = va_arg(args, char *);
                            //掠过空格
                            while (lp_Buffer_Head[0] == ' ')
                                lp_Buffer_Head++;
                            //循环读取内容
                            while (1)
                            {
                                //检查是否结束
                                if (lp_Buffer_Head[0] == ' ' || lp_Buffer_Head[0] == '\0' || lp_Buffer_Head[0] == '\n')
                                    break;
                                else
                                {
                                    //文字拷贝
                                    lpString[0] = lp_Buffer_Head[0];
                                    //递增指针
                                    lpString++;
                                    lp_Buffer_Head++;
                                }
                            }
                            //格式化字符串指针递增
                            lpFormatString += 2;
                            lpBuffer_Head_roll();
                            break;
                        }
                        case 'c':
                        {
                            char *lpChar = va_arg(args, char *);
                            //检查下一位是否结束
                            if (lp_Buffer_Head[0] == '\0')
                                break;
                            else
                            {
                                //读取值
                                lpChar[0] = lp_Buffer_Head[0];
                                //转移缓冲区指针
                                lp_Buffer_Head++;
                                while (lp_Buffer_Head[0] == ' ' || lp_Buffer_Head[0] == '\n')
                                    lp_Buffer_Head++;
                            }
                            //格式化字符串指针递增
                            lpFormatString += 2;
                            break;
                        }
                        case 'd':
                        {
                            //取出指针
                            int *lpNum = va_arg(args, int *);
                            lpNum[0] = 0;
                            //掠过空格
                            while (lp_Buffer_Head[0] == ' ')
                                lp_Buffer_Head++;
                            bool MinusNum = false;
                            //检查是否有正负号
                            if (lp_Buffer_Head[0] == '-')
                            {
                                MinusNum = true;
                                lp_Buffer_Head++;
                            }
                            //循环读取内容
                            while (1)
                            {

                                //检查是否结束
                                if (lp_Buffer_Head[0] < '0' || lp_Buffer_Head[0] > '9')
                                    break;
                                else
                                {
                                    //读取数字
                                    lpNum[0] *= 10;
                                    lpNum[0] += lp_Buffer_Head[0] - '0';
                                    //递增指针
                                    lp_Buffer_Head++;
                                }
                            }
                            //匹配正负号
                            if (MinusNum)
                                lpNum[0] = -lpNum[0];
                            lpFormatString += 2;
                            lpBuffer_Head_roll();
                            break;
                        }
                        case 'f':
                        {
                            //浮点数指针
                            float *lpNum = va_arg(args, float *);
                            //读取整数部分
                            {
                                //临时整型变量，应对arm下sscanf无法读取浮点数及Keil使用sscanf卡住的的问题
                                int temp = 0;
                                //掠过空格
                                while (lp_Buffer_Head[0] == ' ')
                                    lp_Buffer_Head++;
                                bool MinusNum = false;
                                //检查是否有正负号
                                if (lp_Buffer_Head[0] == '-')
                                {
                                    MinusNum = true;
                                    lp_Buffer_Head++;
                                }
                                //循环读取内容
                                while (1)
                                {

                                    //检查是否结束
                                    if (lp_Buffer_Head[0] < '0' || lp_Buffer_Head[0] > '9')
                                        break;
                                    else
                                    {
                                        //读取数字
                                        temp *= 10;
                                        temp += lp_Buffer_Head[0] - '0';
                                        //递增指针
                                        lp_Buffer_Head++;
                                    }
                                }
                                //匹配正负号
                                if (MinusNum)
                                    temp = -temp;

                                //读取浮点数
                                *lpNum = 0.0f + temp;
                            }

                            //缓冲区头指针移动到小数点之后
                            while (1)
                            {
                                if (lp_Buffer_Head[0] == '.') //检查空格
                                {
                                    lp_Buffer_Head++;
                                    if (lp_Buffer_Head[0] == '\0' || lp_Buffer_Head[0] == '\n')
                                        return IO_STREAM_ERROR_SCANF_FAILED;
                                    else
                                        break;
                                }
                                else if (lp_Buffer_Head[0] == ' ') //检查空格
                                    break;
                                else if (lp_Buffer_Head[0] == '\0' || lp_Buffer_Head[0] == '\n')
                                    return IO_STREAM_ERROR_SCANF_FAILED;
                                else
                                    lp_Buffer_Head++;
                            }

                            //筛查是否遇到空格
                            if (lp_Buffer_Head[0] != ' ')
                            {
                                //临时存储浮点值
                                float container;

                                //临时整型变量，应对arm下sscanf无法读取浮点数及Keil使用sscanf卡住的的问题
                                int temp = 0;
                                //掠过空格
                                while (lp_Buffer_Head[0] == ' ')
                                    lp_Buffer_Head++;
                                bool MinusNum = false;
                                //检查是否有正负号
                                if (lp_Buffer_Head[0] == '-')
                                {
                                    MinusNum = true;
                                    lp_Buffer_Head++;
                                }
                                //循环读取内容
                                while (1)
                                {

                                    //检查是否结束
                                    if (lp_Buffer_Head[0] < '0' || lp_Buffer_Head[0] > '9')
                                        break;
                                    else
                                    {
                                        //读取数字
                                        temp *= 10;
                                        temp += lp_Buffer_Head[0] - '0';
                                        //递增指针
                                        lp_Buffer_Head++;
                                    }
                                }
                                //匹配正负号
                                if (MinusNum)
                                    temp = -temp;
                                //临时值
                                container = 0.0f + temp;
                                //循环降次
                                for (int count = temp; count > 0; count /= 10)
                                    container /= 10;

                                //加上小数部分
                                *lpNum += container;
                            }
                            lpFormatString += 2;
                            lpBuffer_Head_roll();
                            break;
                        }
                        case 'l':
                            if (lpFormatString[2] == 'f')
                            {
                                //浮点数指针
                                double *lpNum = va_arg(args, double *);
                                //读取整数部分
                                {
                                    //临时整型变量，应对arm下sscanf无法读取浮点数及Keil使用sscanf卡住的的问题
                                    int temp = 0;
                                    //掠过空格
                                    while (lp_Buffer_Head[0] == ' ')
                                        lp_Buffer_Head++;
                                    bool MinusNum = false;
                                    //检查是否有正负号
                                    if (lp_Buffer_Head[0] == '-')
                                    {
                                        MinusNum = true;
                                        lp_Buffer_Head++;
                                    }
                                    //循环读取内容
                                    while (1)
                                    {

                                        //检查是否结束
                                        if (lp_Buffer_Head[0] < '0' || lp_Buffer_Head[0] > '9')
                                            break;
                                        else
                                        {
                                            //读取数字
                                            temp *= 10;
                                            temp += lp_Buffer_Head[0] - '0';
                                            //递增指针
                                            lp_Buffer_Head++;
                                        }
                                    }
                                    //匹配正负号
                                    if (MinusNum)
                                        temp = -temp;

                                    //读取浮点数
                                    *lpNum = 0.0f + temp;
                                }

                                //缓冲区头指针移动到小数点之后
                                while (1)
                                {
                                    if (lp_Buffer_Head[0] == '.') //检查空格
                                    {
                                        lp_Buffer_Head++;
                                        if (lp_Buffer_Head[0] == '\0' || lp_Buffer_Head[0] == '\n')
                                            return IO_STREAM_ERROR_SCANF_FAILED;
                                        else
                                            break;
                                    }
                                    else if (lp_Buffer_Head[0] == ' ') //检查空格
                                        break;
                                    else if (lp_Buffer_Head[0] == '\0' || lp_Buffer_Head[0] == '\n')
                                        return IO_STREAM_ERROR_SCANF_FAILED;
                                    else
                                        lp_Buffer_Head++;
                                }

                                //筛查是否遇到空格
                                if (lp_Buffer_Head[0] != ' ')
                                {
                                    //临时存储浮点值
                                    float container;

                                    //临时整型变量，应对arm下sscanf无法读取浮点数及Keil使用sscanf卡住的的问题
                                    int temp = 0;
                                    //掠过空格
                                    while (lp_Buffer_Head[0] == ' ')
                                        lp_Buffer_Head++;
                                    bool MinusNum = false;
                                    //检查是否有正负号
                                    if (lp_Buffer_Head[0] == '-')
                                    {
                                        MinusNum = true;
                                        lp_Buffer_Head++;
                                    }
                                    //循环读取内容
                                    while (1)
                                    {

                                        //检查是否结束
                                        if (lp_Buffer_Head[0] < '0' || lp_Buffer_Head[0] > '9')
                                            break;
                                        else
                                        {
                                            //读取数字
                                            temp *= 10;
                                            temp += lp_Buffer_Head[0] - '0';
                                            //递增指针
                                            lp_Buffer_Head++;
                                        }
                                    }
                                    //匹配正负号
                                    if (MinusNum)
                                        temp = -temp;
                                    //临时值
                                    container = 0.0f + temp;
                                    //循环降次
                                    for (int count = temp; count > 0; count /= 10)
                                        container /= 10;

                                    //加上小数部分
                                    *lpNum += container;
                                }
                                lpFormatString += 3;
                                lpBuffer_Head_roll();
                                break;
                            }
                        default:
                            lpFormatString++;
                        }

                        //读取到结尾刷新缓冲区
                        if (lp_Buffer_Head[0] == '\0')
                            flush();
                    }
                }
                else
                    return IO_STREAM_ERROR_SCANF_FAILED;
            }
            return IO_STREAM_ERROR_NONE;
        }
        else
            return IO_STREAM_ERROR_UNINITED;
    }

    /**
     * 开启结束字符中断
     * @param End_Char 定义的结束字符
     * @param lpEUSCI_Ax_End_CallBack 自定义的中断回调函数指针
     * @arg 必须是void 返回值，参数为void的函数
     */
    void MSP432_Uart::Enable_EndChar_Interrupt(const char End_Char, void (*lpEUSCI_Ax_End_CallBack)(void))
    {
        if (isInit_already)
        {
            //清理缓冲区
            // flush();
            //设置定义
            switch (EUSCI_Ax_BASE)
            {
            case EUSCI_A0_BASE:
                EUSCI_A0_End_Char = End_Char;
                lpEUSCI_A0_End_CallBack = lpEUSCI_Ax_End_CallBack;
                break;
            case EUSCI_A1_BASE:
                EUSCI_A1_End_Char = End_Char;
                lpEUSCI_A1_End_CallBack = lpEUSCI_Ax_End_CallBack;
                break;
            case EUSCI_A2_BASE:
                EUSCI_A2_End_Char = End_Char;
                lpEUSCI_A2_End_CallBack = lpEUSCI_Ax_End_CallBack;
                break;
            case EUSCI_A3_BASE:
                EUSCI_A3_End_Char = End_Char;
                lpEUSCI_A3_End_CallBack = lpEUSCI_Ax_End_CallBack;
                break;
            }
        }
    }

    //清理缓冲区
    void MSP432_Uart::flush()
    {
        //初始化为0并设置初始化指针
        memset(buffer, '\0', MSP432_UART_RECEIVE_BUFFER_LENGTH);
        buffer_counter = 0;
        lp_Buffer_Head = buffer;
    }
}
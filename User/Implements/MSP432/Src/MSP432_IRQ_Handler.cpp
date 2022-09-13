#include "MSP432_IRQ_Handler.hpp"

namespace cus
{

    /**
     * 启用引脚外部中断
     * @param GPIOx 引脚区域
     * @param GPIO_Pin 引脚编号
     */
    void ENA_GPIO_EXIT_Interrupt(uint_fast8_t GPIOx, uint_fast16_t GPIO_Pin)
    {
        switch (GPIOx)
        {
        case GPIO_PORT_P1:
        case GPIO_PORT_P2:
        case GPIO_PORT_P3:
        case GPIO_PORT_P4:
        case GPIO_PORT_P5:
        case GPIO_PORT_P6:
            //设置为上拉输入模式
            GPIO_setAsInputPinWithPullUpResistor(GPIOx, GPIO_Pin);
            //清除中断标志位
            GPIO_clearInterruptFlag(GPIOx, GPIO_Pin);
            //设置中断触发为高电平跳低电平
            GPIO_interruptEdgeSelect(GPIOx, GPIO_Pin, GPIO_HIGH_TO_LOW_TRANSITION);
            //启用引脚外部中断
            GPIO_enableInterrupt(GPIOx, GPIO_Pin);
            //启用引脚区域中断
            Interrupt_enableInterrupt(INT_PORT1 + (GPIOx - GPIO_PORT_P1));
            //启用总中断
            Interrupt_enableMaster();
        default:
            return;
        }
    }

    /**
     * 引脚外部中断回调函数，可重定义
     * @param GPIOx 引脚区域
     * @param GPIO_Pin 引脚编号
     */
    __WEAK void GPIO_EXIT_Interrupt_CallBack(uint_fast8_t GPIOx, uint_fast16_t GPIO_Pin)
    {
        //自由发挥，可强定义
    }
}

//引脚端口1中断入口函数
extern "C" void PORT1_IRQHandler(void)
{
    //获取中断状态
    uint16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

    //循环比对是哪一位中断并调用中断回调
    for (int count = 0; count < 16; count++)
        if (status & (GPIO_PIN1 << count))
            cus::GPIO_EXIT_Interrupt_CallBack(GPIO_PORT_P1, GPIO_PIN1 << count);

    //清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
}

//引脚端口2中断入口函数
extern "C" void PORT2_IRQHandler(void)
{
    //获取中断状态
    uint16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);

    //循环比对是哪一位中断并调用中断回调
    for (int count = 0; count < 16; count++)
        if (status & (GPIO_PIN1 << count))
            cus::GPIO_EXIT_Interrupt_CallBack(GPIO_PORT_P2, GPIO_PIN1 << count);

    //清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P2, status);
}

//引脚端口3中断入口函数
extern "C" void PORT3_IRQHandler(void)
{
    //获取中断状态
    uint16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);

    //循环比对是哪一位中断并调用中断回调
    for (int count = 0; count < 16; count++)
        if (status & (GPIO_PIN1 << count))
            cus::GPIO_EXIT_Interrupt_CallBack(GPIO_PORT_P3, GPIO_PIN1 << count);

    //清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
}

//引脚端口4中断入口函数
extern "C" void PORT4_IRQHandler(void)
{
    //获取中断状态
    uint16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);

    //循环比对是哪一位中断并调用中断回调
    for (int count = 0; count < 16; count++)
        if (status & (GPIO_PIN1 << count))
            cus::GPIO_EXIT_Interrupt_CallBack(GPIO_PORT_P4, GPIO_PIN1 << count);

    //清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);
}

//引脚端口5中断入口函数
extern "C" void PORT5_IRQHandler(void)
{
    //获取中断状态
    uint16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);

    //循环比对是哪一位中断并调用中断回调
    for (int count = 0; count < 16; count++)
        if (status & (GPIO_PIN1 << count))
            cus::GPIO_EXIT_Interrupt_CallBack(GPIO_PORT_P5, GPIO_PIN1 << count);

    //清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
}

//引脚端口6中断入口函数
extern "C" void PORT6_IRQHandler(void)
{
    //获取中断状态
    uint16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);

    //循环比对是哪一位中断并调用中断回调
    for (int count = 0; count < 16; count++)
        if (status & (GPIO_PIN1 << count))
            cus::GPIO_EXIT_Interrupt_CallBack(GPIO_PORT_P6, GPIO_PIN1 << count);

    //清除中断标志
    GPIO_clearInterruptFlag(GPIO_PORT_P6, status);
}
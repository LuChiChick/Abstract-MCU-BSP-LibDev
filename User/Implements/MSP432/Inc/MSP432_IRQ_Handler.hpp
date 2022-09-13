#ifndef __CUS__IRQ_HANDLER__
#define __CUS__IRQ_HANDLER__

extern "C"
{
#include "driverlib.h"
}

namespace cus
{
    /**
     * 启用引脚外部中断
     * @param GPIOx 引脚区域
     * @param GPIO_Pin 引脚编号
     */
    void ENA_GPIO_EXIT_Interrupt(uint_fast8_t GPIOx, uint_fast16_t GPIO_Pin);

    /**
     * 引脚外部中断回调函数，可重定义
     * @param GPIOx 引脚区域
     * @param GPIO_Pin 引脚编号
     */
    void GPIO_EXIT_Interrupt_CallBack(uint_fast8_t GPIOx, uint_fast16_t GPIO_Pin);

}
#endif
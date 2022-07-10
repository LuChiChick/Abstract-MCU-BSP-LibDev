#ifndef __CUS__STM32_WIRE__
#define __CUS__STM32_WIRE__

#include "Abs_BaseIO.hpp"
extern "C"
{
#include "main.h"
}

namespace cus
{
    // GPIO类具体实现
    class STM32_GPIO_Wire : public abs_GPIO_Wire
    {
    protected:
        bool isInit_already; //是否初正确构造
        GPIO_Wire_Mode Mode; //内部信息记录-引脚模式
        GPIO_TypeDef *GPIOx; //内部信息记录-引脚区域
        uint16_t GPIO_Pin;   //内部信息记录-引脚编号

    public:
        //默认构造函数
        STM32_GPIO_Wire();
        //带参构造函数
        STM32_GPIO_Wire(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

        /**
         * 构造&类初始化
         * @param GPIOx 引脚区域
         * @param GPIO_Pin 引脚编号
         * @return abs_Wire实例
         */
        virtual STM32_GPIO_Wire &construct(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

        /**
         *   设置引脚输出模式
         *   @param Mode -引脚模式,
         *   详见GPIO_Mode枚举类型
         *   @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error setAs(GPIO_Wire_Mode Mode);

        /**
         *   输出电平
         *   @param Level -决定输出的电平高低
         *   详见GPIO_Level枚举类型
         *   @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error write(GPIO_Wire_Level Level);

        /**
         *   读取电平高低
         *   @return GPIO_Wire_Level 电平高低
         */
        virtual GPIO_Wire_Level readLevel();

        /**
         *   获取当前模式
         *   @return GPIO_Wire_Mode 引脚模式
         */
        virtual GPIO_Wire_Mode getMode();

        /**
         * 翻转电平
         * @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error toggle();

        //释放资源
        virtual GPIO_Wire_Error release();
    };
}
#endif
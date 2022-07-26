#ifndef __CUS__MSP432_PWM_GENERATOR__
#define __CUS__MSP432_PWM_GENERATOR__

//默认定时器频率
#define DEF_TIMER_FREQUENCY 1000000
#define DEF_TIMER_PERIOD 20000
//驱动库包含
extern "C"
{
#include "driverlib.h"
}
#include "Abs_PWM_Generator.hpp"

namespace cus
{
    // MSP432 PWM生成器 类实现
    class MSP432_PWM_Generator : public Abs_PWM_Generator
    {
    private:
        //初始化审查
        bool isInit_already;
        // Timer 配置结构体
        Timer_A_PWMConfig config;
        //所在定时器
        uint32_t TIMER_Ax_BASE;
        //所在定时器捕获通道
        uint32_t TIMER_A_CAPTURECOMPARE_REGISTER;

    public:
        //默认构造函数
        MSP432_PWM_Generator();
        /**
         * 带参构造函数
         * @param TIMER_Ax_BASE 所在定时器
         * @param TIMER_A_CAPTURECOMPARE_REGISTER 所在定时器捕获通道
         * @param ... 上述参数组合如下
         * @param -TA0.0 ->P7.3
         * @param -TA0.1 ->P2.4
         * @param -TA0.2 ->P2.5
         * @param -TA0.3 ->P2.6
         * @param -TA0.4 ->P2.7
         * @param -TA1.0 ->P8.0
         * @param -TA1.1 ->P7.7
         * @param -TA1.2 ->P7.6
         * @param -TA1.3 ->P7.5
         * @param -TA1.4 ->P7.4
         * @param -TA2.0 ->P8.1
         * @param -TA2.1 ->P5.6
         * @param -TA2.2 ->P5.7
         * @param -TA2.3 ->P6.6
         * @param -TA2.4 ->P6.7
         * @param -TA3.0 ->P10.4
         * @param -TA3.1 ->P10.5
         * @param -TA3.2 ->P8.2
         * @param -TA3.3 ->P9.2
         * @param -TA3.4 ->P9.3
         */
        MSP432_PWM_Generator(uint32_t TIMER_Ax_BASE, uint32_t TIMER_A_CAPTURECOMPARE_REGISTER);

        /**
         * 构造&初始化
         * @param TIMER_Ax_BASE 所在定时器
         * @param TIMER_A_CAPTURECOMPARE_REGISTER 所在定时器捕获通道
         * @return MSP432_PWM_Generator实例
         * @param ... 上述参数组合如下
         * @param -TA0.0 ->P7.3
         * @param -TA0.1 ->P2.4
         * @param -TA0.2 ->P2.5
         * @param -TA0.3 ->P2.6
         * @param -TA0.4 ->P2.7
         * @param -TA1.0 ->P8.0
         * @param -TA1.1 ->P7.7
         * @param -TA1.2 ->P7.6
         * @param -TA1.3 ->P7.5
         * @param -TA1.4 ->P7.4
         * @param -TA2.0 ->P8.1
         * @param -TA2.1 ->P5.6
         * @param -TA2.2 ->P5.7
         * @param -TA2.3 ->P6.6
         * @param -TA2.4 ->P6.7
         * @param -TA3.0 ->P10.4
         * @param -TA3.1 ->P10.5
         * @param -TA3.2 ->P8.2
         * @param -TA3.3 ->P9.2
         * @param -TA3.4 ->P9.3
         */
        MSP432_PWM_Generator &construct(uint32_t TIMER_Ax_BASE, uint32_t TIMER_A_CAPTURECOMPARE_REGISTER);
        /**
         * 设置PWM信号周期
         * @param Period 周期
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error Set_Period(uint32_t Period);

        /**
         * 设置PWM信号占空周期
         * @param DutyCycle 占空周期
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error Set_DutyCycle(uint32_t Period);

        /**
         * 开始输出
         * @param 无
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error start();

        /**
         * 停止输出
         * @param 无
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error stop();
    };
}

#endif

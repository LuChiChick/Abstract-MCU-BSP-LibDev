#include "MSP432_PWM_Generator.hpp"

namespace cus
{
    //默认构造函数
    MSP432_PWM_Generator::MSP432_PWM_Generator()
    {
        isInit_already = false;
    }

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
    MSP432_PWM_Generator::MSP432_PWM_Generator(uint32_t TIMER_Ax_BASE, uint32_t TIMER_A_CAPTURECOMPARE_REGISTER)
    {
        construct(TIMER_Ax_BASE, TIMER_A_CAPTURECOMPARE_REGISTER);
    }

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
    MSP432_PWM_Generator &MSP432_PWM_Generator::construct(uint32_t TIMER_Ax_BASE, uint32_t TIMER_A_CAPTURECOMPARE_REGISTER)
    {
        //检查定时器外设
        if ((TIMER_Ax_BASE == TIMER_A0_BASE) ||
            (TIMER_Ax_BASE == TIMER_A1_BASE) ||
            (TIMER_Ax_BASE == TIMER_A2_BASE) ||
            (TIMER_Ax_BASE == TIMER_A3_BASE))
            if ((TIMER_A_CAPTURECOMPARE_REGISTER == TIMER_A_CAPTURECOMPARE_REGISTER_0) ||
                (TIMER_A_CAPTURECOMPARE_REGISTER == TIMER_A_CAPTURECOMPARE_REGISTER_1) ||
                (TIMER_A_CAPTURECOMPARE_REGISTER == TIMER_A_CAPTURECOMPARE_REGISTER_2) ||
                (TIMER_A_CAPTURECOMPARE_REGISTER == TIMER_A_CAPTURECOMPARE_REGISTER_3) ||
                (TIMER_A_CAPTURECOMPARE_REGISTER == TIMER_A_CAPTURECOMPARE_REGISTER_4))
            {

                config.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                    //选择时钟源
                config.clockSourceDivider = SystemCoreClock / DEF_TIMER_FREQUENCY; //分频产生1Mhz;
                config.timerPeriod = DEF_TIMER_PERIOD;
                //指定结构体
                config.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER;
                config.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;
                config.dutyCycle = 0;

                // A0
                if (TIMER_Ax_BASE == TIMER_A0_BASE)
                {
                }

                //开始生成PWM
                Timer_A_generatePWM(TIMER_A0_BASE, &config);
            }
        return *this;
    }
}
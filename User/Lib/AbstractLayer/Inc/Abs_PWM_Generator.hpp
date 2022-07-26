#ifndef __CUS__ABS_PWM_GENERATOR__
#define __CUS__ABS_PWM_GENERATOR__

#include "stdint.h"

namespace cus
{
    // PWM生成器错误类型枚举
    enum PWM_Generator_Error
    {
        PWM_GENERATOR_ERROR_NONE,         //无错误
        PWM_GENERATOR_ERROR_UNINITED,     //未初始化
        PWM_GENERATOR_INCORRECT_ARGUMENT, //参数不正确
    };

    //抽象PWM生成器
    class Abs_PWM_Generator
    {
    public:
        /**
         * 设置PWM信号周期
         * @param Period 周期
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error Set_Period(uint32_t Period) = 0;

        /**
         * 设置PWM信号占空周期
         * @param DutyCycle 占空周期
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error Set_DutyCycle(uint32_t Period) = 0;

        /**
         * 开始输出
         * @param 无
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error start() = 0;

        /**
         * 停止输出
         * @param 无
         * @return  PWM_Generator_Error错误抛出
         */
        virtual PWM_Generator_Error stop() = 0;
    };
}

#endif

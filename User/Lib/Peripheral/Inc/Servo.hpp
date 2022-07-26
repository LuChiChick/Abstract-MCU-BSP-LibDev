#ifndef __CUS__SERVO__
#define __CUS__SERVO__

#include "Abs_PWM_Generator.hpp"
#include "Calculate.hpp"

namespace cus
{
    class Servo
    {
    private:
        bool isInit_already; //初始化审查

        //类内PWM生成器记录
        Abs_PWM_Generator *PWM_Generator;

        int MAX_COMPARE; //占空比tic最大值
        int MIN_COMPARE; //占空比tic最小值
        float MAX_ANGLE; //最大角度

        float angle; //当前角度
    public:
        //默认构造函数
        Servo();

        /**
         * 带参构造函数
         * @param PWM_Generator PWM生成器类实例
         * @param MAX_ANGLE 最大角度
         */
        Servo(Abs_PWM_Generator &PWM_Generator, float MAX_ANGLE);

        /**
         * 构造&初始化
         * @param PWM_Generator PWM生成器类实例
         * @param MAX_ANGLE 最大角度
         * @return Servo实例
         */
        Servo &construct(Abs_PWM_Generator &PWM_Generator, float MAX_ANGLE);

        /**
         * 获取舵机角度
         * @return 角度
         */
        virtual float getAngle();

        /**
         * 移动到指定角度
         * @param angle 最大角度
         * @return 无
         */
        virtual void attach(float angle);
    };
}

#endif

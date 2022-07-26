#include "Servo.hpp"

//默认分频系数
#define DEF_SERVO_DIVIDER TIMER_A_CLOCKSOURCE_DIVIDER_48
//默认舵机周期计数
#define DEF_SERVO_PERIOD 20000

namespace cus
{
    //默认构造函数
    Servo::Servo()
    {
        isInit_already = false;
    }

    /**
     * 带参构造函数
     * @param PWM_Generator PWM生成器类实例
     * @param MAX_ANGLE 最大角度
     */
    Servo::Servo(Abs_PWM_Generator &PWM_Generator, float MAX_ANGLE)
    {
        construct(PWM_Generator, MAX_ANGLE);
    }

    /**
     * 构造&初始化
     * @param PWM_Generator PWM生成器类实例
     * @param MAX_ANGLE 最大角度
     * @return Servo实例
     */
    Servo &Servo::construct(Abs_PWM_Generator &PWM_Generator, float MAX_ANGLE)
    {
        //信息记录
        this->PWM_Generator = &PWM_Generator;
        this->MAX_COMPARE = (DEF_SERVO_PERIOD * 0.125);
        this->MIN_COMPARE = (DEF_SERVO_PERIOD * 0.025);
        this->MAX_ANGLE = MAX_ANGLE;

        //设置周期
        PWM_Generator.Set_Period(DEF_SERVO_PERIOD);

        //角度初始化
        angle = -1;
        //完成初始化
        isInit_already = true;
        return *this;
    }

    /**
     * 移动到指定角度
     * @param angle 最大角度
     * @return 无
     */
    void Servo::attach(float angle)
    {
        if (isInit_already && angle <= MAX_ANGLE && angle >= 0)
        {
            PWM_Generator->start();
            PWM_Generator->Set_DutyCycle(cus::map(angle, 0.0f, MAX_ANGLE, MIN_COMPARE, MAX_COMPARE));
            //记录数据
            this->angle = angle;
        }
    }
}
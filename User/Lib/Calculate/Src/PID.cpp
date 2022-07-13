#include "Calculate.hpp"

namespace cus
{
    //默认构造函数
    float_PID::float_PID()
    {
        //初始化审查
        inInit_already = false;
    }

    /**
     * 带参构造函数
     * @param kP 误差直接反馈比率
     * @param kI 误差积分反馈比率
     * @param kD 误差微分反馈比率
     * @param max 输出最大值
     * @param min 输出最小值
     */
    float_PID::float_PID(float kP, float kI, float kD, float max, float min)
    {
        //数据记录
        this->kP = kP;
        this->kI = kI;
        this->kD = kD;

        //范围记录
        this->max = max;
        this->min = min;

        //数据清零
        pre_error = 0;
        current_err = 0;
        err_integral = 0;

        //初始化审查
        inInit_already = true;
    }

    /**
     * 计算PID得数
     * @param target 目标值
     * @param current 当前测量值
     * @return 输出值
     */
    float float_PID::calculate(float target, float measure)
    {
        if (inInit_already) //正确初始化审查
        {
            float P_out, I_out, D_out, Total_out; //三项输出及输出总和
            
            current_err = target - measure;       //当前误差计算
            err_integral += current_err;          //误差累积

            P_out = kP * current_err;               //比例项
            I_out = kI * err_integral;              //积分项
            D_out = kD * (current_err - pre_error); //微分项

            Total_out = P_out + I_out + D_out; //PID输出

            pre_error = current_err; //更改前值误差

            //死区判断
            if (Total_out > max)
                return max;
            if (Total_out < min)
                return min;

            //输出
            return Total_out;
        }
        else
            return 0.0f;
    }

    /**
     * 清除误差积分
     * @param 无
     * @return 无
     */
    void float_PID::clear_err_integral(void)
    {
        if (inInit_already)
            err_integral = 0;
    }

}

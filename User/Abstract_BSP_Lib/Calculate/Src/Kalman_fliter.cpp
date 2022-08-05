#include "Calculate.hpp"

namespace cus
{
    //默认构造函数
    Kalman_fliter::Kalman_fliter()
    {
        reset();
    }

    /**
     * 计算输出值,需要稳定计算周期
     * @param input 输入值
     * @return 滤波后输出值
     */
    float Kalman_fliter::calculate(float input)
    {
        //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
        Now_P = Last_P + Q;

        //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
        Kg = Now_P / (Now_P + R);

        //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）

        out = out + Kg * (input - out); //因为这一次的预测值就是上一次的输出值

        //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
        Last_P = (1 - Kg) * Now_P;

        return out;
    }

    //重置输出
    void Kalman_fliter::reset()
    {
        Last_P = 1;
        Now_P = 0;
        out = 0;
        Kg = 0;
        Q = 0;
        R = 0.01;
    }

}
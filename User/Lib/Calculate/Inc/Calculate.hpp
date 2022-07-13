#ifndef __CUS__CALCULATE__
#define __CUS__CALCULATE__

namespace cus
{
    /**
     * 范围映射工具函数模板
     * @param input 输入的值
     * @param input_min 输入值的最小范围值
     * @param input_max 输入值的最大范围值
     * @param output_min 输出值的最小范围值
     * @param output_max 输出值的最大范围值
     * @return 映射后的范围值,类型为输出值类型
     */
    template <class inputType, class outputType>
    outputType map(inputType input, inputType input_min, inputType input_max, outputType output_min, outputType output_max)
    {
        //简单的施密特范围映射
        return (outputType)(input - input_min) * (output_max - output_min) / (outputType)(input_max - input_min) + output_min;
    }

    /**
     * @brief  快速平方根倒数算法，计算 1/Sqrt(x)
     * @param  x 需要计算的数
     * @return 1/Sqrt(x)
     */
    float inv_sqrt(float x);

    //浮点PID内嵌算法类
    class float_PID
    {
    private:
        bool inInit_already; //初始化审查

        float max; //最小值
        float min; //最大值

        float current_err;  //当前误差
        float pre_error;    //前误差
        float err_integral; //误差积分

        float kP; //比率系数
        float kI; //积分比率
        float kD; //微分系数

    public:
        //默认构造函数
        float_PID();

        /**
         * 带参构造函数
         * @param kP 误差直接反馈比率
         * @param kI 误差积分反馈比率
         * @param kD 误差微分反馈比率
         * @param max 输出最大值
         * @param min 输出最小值
         */
        float_PID(float kP, float kI, float kD, float max, float min);

        /**
         * 计算PID得数
         * @param target 目标值
         * @param measure 测量值
         * @return 输出值
         */
        float calculate(float target, float measure);

        /**
         * 清除误差积分
         * @param 无
         * @return 无
         */
        void clear_err_integral(void);
    };

    //卡尔曼滤波类
    class Kalman_fliter
    {
    private:
        float Last_P; //上次估算协方差 不可以为0 ! ! ! ! !
        float Now_P;  //当前估算协方差
        float out;    //卡尔曼滤波器输出
        float Kg;     //卡尔曼增益
        float Q;      //过程噪声协方差
        float R;      //观测噪声协方差
    public:
        //构造函数
        Kalman_fliter();

        /**
         * 计算输出值,需要稳定计算周期
         * @param input 输入值
         * @return 滤波后输出值
         */
        float calculate(float input);

        //重置输出
        void reset();
    };
}

#endif

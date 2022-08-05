#include "Calculate.hpp"
namespace cus
{
    /**
     * @brief  快速平方根倒数算法，计算 1/Sqrt(x)
     * @param  x 需要计算的数
     * @return 1/Sqrt(x)
     */
    float inv_sqrt(float x)
    {
        float halfx = 0.5f * x;
        float y = x;
        long i = *(long *)&y;

        i = 0x5f3759df - (i >> 1);
        y = *(float *)&i;
        y = y * (1.5f - (halfx * y * y));

        return y;
    }
}
#include "Abs_Monochrome_Screen.hpp"

namespace cus
{
    /**
     * 设置字体大小
     * @param font_size
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::set_Font_Size(uint8_t font_size)
    {
        this->font_size = font_size;
        //返回无错误
        return MONOCHROME_SCREEN_ERROR_NONE;
    }

    /**
     * 反转像素颜色
     * @param false_for_Normal_true_for_Revers false正常像素点true反色像素点
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::reverse_Pixel_Color(bool false_for_Normal_true_for_Revers)
    {
        //记录笔刷切换与否
        Pixel_Color_Reverse = false_for_Normal_true_for_Revers;
        //返回无错误
        return MONOCHROME_SCREEN_ERROR_NONE;
    }
}
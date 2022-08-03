#ifndef __CUS__MONOCHROME_SCREEN__
#define __CUS__MONOCHROME_SCREEN__
#include "stdint.h"

namespace cus
{
    //单色屏驱动类库错误抛出类型
    enum Monochrome_Screen_Error
    {
        MONOCHROME_SCREEN_ERROR_NONE,           //无错误
        MONOCHROME_SCREEN_ERROR_UNINITED,       //未初始化
        MONOCHROME_SCREEN_ERROR_NOBUFFER,       //没有缓冲区
        MONOCHROME_SCREEN_ERROR_NO_FULLBUFFER,  //没有全页缓存
        MONOCHROME_ERROR_BUFFER_SIZE_INCORRECT, //缓冲区大小不正确
        MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_X, // X出界
        MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y, // Y出界
    };

    //单色屏驱动类库
    class Monochrome_Screen
    {
    protected:
        bool isInit_already; //是否初正确构造

        uint16_t SCREEN_X; //屏幕长
        uint16_t SCREEN_Y; //屏幕宽

    public:
        Monochrome_Screen();
        /**
         * 绘制像素点
         * @param X X坐标
         * @param Y Y坐标
         * @return Monochrome_Screen_Error错误类型抛出
         */
        virtual Monochrome_Screen_Error draw_Pixel(uint16_t X, uint16_t Y) = 0;
    };

}

#endif

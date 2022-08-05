#ifndef __CUS__ABS_MONOCHROME_SCREEN__
#define __CUS__ABS_MONOCHROME_SCREEN__

//抽象IO流包含
#include "Abs_IO_Stream.hpp"
//标准整形包含
#include "stdint.h"

namespace cus
{
    //单色屏驱动类库错误异常抛出类型
    enum Monochrome_Screen_Error
    {
        MONOCHROME_SCREEN_ERROR_NONE,                  //无错误
        MONOCHROME_SCREEN_ERROR_UNINITED,              //未初始化
        MONOCHROME_SCREEN_ERROR_NOBUFFER,              //没有缓冲区
        MONOCHROME_SCREEN_ERROR_NO_FULLBUFFER,         //没有全页缓存
        MONOCHROME_SCREEN_ERROR_BUFFER_SIZE_INCORRECT, //缓冲区大小不正确
        MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_X,        // X出界
        MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y,        // Y出界
        MONOCHROME_SCREEN_ERROR_UNSUPPORTED_FONT_SIZE, //不支持的字体大小
        MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED,      //输出流失败
        MONOCHROME_SCREEN_ERROR_FUNCTION_UNREALIZED,   //功能未实现
    };

    //单色屏驱动类库,继承输出流功能
    class Monochrome_Screen : public Abs_OStream
    {
    protected:
        uint16_t SCREEN_X; //屏幕长
        uint16_t SCREEN_Y; //屏幕宽

        uint8_t *buffer;   //缓冲区
        bool isFullBuffer; //是否全页缓存

        bool Brush_Color_Switch; //笔刷反色

        /**
         *   初始化屏幕
         *   @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error init_Screen() = 0;

    public:
        //默认构造函数
        Monochrome_Screen();

        /**
         * 清理屏幕
         * @param 无
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error clear() = 0;

        /**
         * 切换笔刷颜色
         * @param false_for_Normal_true_for_Revers false正常笔刷true反色笔刷
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error switch_Brush_Color(bool false_for_Normal_true_for_Revers) = 0;

        /**
         * 设置字体大小
         * @param font_size
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error set_Font_Size(uint8_t font_size) = 0;

        /**
         * 绘制像素点
         * @param x_offest X坐标
         * @param y_offest Y坐标
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_Pixel(uint16_t x_offest, uint16_t y_offest) = 0;

        /**
         * 向默认位置(0,0)绘制图形
         * @param length 图片像素长度
         * @param width 图片像素宽度
         * @param IMG_Arr 图片数组,每列8位点阵,字节垂直数据水平
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_IMG(uint16_t length, uint16_t width, const uint8_t *IMG_Arr) = 0;

        /**
         * 指定位置输出图片
         * @param x_offest X坐标
         * @param y_offest y坐标
         * @param length 图片像素长度
         * @param width 图片像素宽度
         * @param IMG_Arr 图片数组
         */
        virtual Monochrome_Screen_Error draw_IMG(uint16_t x_offest, uint16_t y_offest, uint16_t length, uint16_t width, const uint8_t *IMG_Arr) = 0;
    };

}

#endif
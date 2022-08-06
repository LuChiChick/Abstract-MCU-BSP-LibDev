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
        MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED,     //连接失败
        MONOCHROME_SCREEN_ERROR_CLEAR_SCREEN_FAILED,   //清理屏幕失败
        MONOCHROME_SCREEN_ERROR_FUNCTION_UNREALIZED,   //功能未实现
    };

    //单色屏驱动类库,继承输出流功能
    class Abs_Monochrome_Screen : public Abs_OStream
    {
    protected:
        uint16_t SCREEN_X; //屏幕长
        uint16_t SCREEN_Y; //屏幕宽

        uint8_t *buffer;                //缓冲区
        uint16_t Occupied_BufferLength; //已记载的缓冲区长度

        uint8_t font_size; //字体大小

        bool isFullBuffer;        //是否全页缓存
        bool Pixel_Color_Reverse; //笔刷反色

        /**
         *   初始化屏幕
         *   @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error init_Screen() = 0;

    public:
        //默认构造函数
        Abs_Monochrome_Screen();

        /**
         * 清理屏幕
         * @param 无
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error clear() = 0;

        /**
         * 反转像素颜色
         * @param false_for_Normal_true_for_Revers false正常像素点true反色像素点
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error reverse_Pixel_Color(bool false_for_Normal_true_for_Revers);

        /**
         * 设置字体大小
         * @param font_size
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error set_Font_Size(uint8_t font_size);

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
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_IMG(uint16_t x_offest, uint16_t y_offest, uint16_t length, uint16_t width, const uint8_t *IMG_Arr) = 0;

        /**
         *   指定坐标打印字符
         *   @param x_offest x坐标
         *   @param y_offest y坐标
         *   @param chr 输出字符
         *   @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error putchar(uint8_t x_offest, uint8_t y_offest, const char chr) = 0;
    };

}

//单色屏资源区，字体及图像
namespace Monochrome_Screen_Resources
{
    //字节垂直，数据水平，标准ASCII字符6*8点阵
    extern const uint8_t ASCII_0806[92][6];
}

#endif

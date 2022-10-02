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
        MONOCHROME_SCREEN_ERROR_NONE,                    //无错误
        MONOCHROME_SCREEN_ERROR_UNINITED,                //未初始化
        MONOCHROME_SCREEN_ERROR_NOBUFFER,                //没有缓冲区
        MONOCHROME_SCREEN_ERROR_NO_FULLBUFFER,           //没有全页缓存
        MONOCHROME_SCREEN_ERROR_BUFFER_SIZE_INCORRECT,   //缓冲区大小不正确
        MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_X,          // X出界
        MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y,          // Y出界
        MONOCHROME_SCREEN_ERROR_UNSUPPORTED_FONT_FAMILY, //不支持的字体族
        MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED,        //输出流失败
        MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED,       //连接失败
        MONOCHROME_SCREEN_ERROR_CLEAR_SCREEN_FAILED,     //清理屏幕失败
        MONOCHROME_SCREEN_ERROR_FUNCTION_UNREALIZED,     //功能未实现
    };

    //单色屏驱动类库,继承输出流功能
    class Abs_Monochrome_Screen : public Abs_OStream
    {
    protected:
        bool isInit_already; //是否初正确构造

        uint16_t SCREEN_X; //屏幕长
        uint16_t SCREEN_Y; //屏幕宽

        const uint8_t *font_Family; //字体族指针地址，用以调取字体资源
        uint8_t Font_PosLength;     //当前字体像素长度
        uint8_t Font_PosWidth;      //当前像素宽度

        uint16_t cursor_X; // X指针，用于内部记录实现控制台输出
        uint16_t cursor_Y; // Y指针，用于内部记录实现控制台输出

        bool isFullBuffer; //是否全页缓存
        uint8_t *buffer;   //缓冲区地址

        bool Pixel_Color_Reverse; //笔刷反色
        bool Pixel_Overlay;       //像素颜色覆盖（即像素点0值是否会强制覆盖其下方的有色1值像素点）

        /**
         * 初始化屏幕
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error init_Screen() = 0;

        /**
         * 缓存字符数据到缓冲区指定位置
         * @param x_offest X坐标
         * @param y_offest Y坐标
         * @param data 需要缓存的数据
         * @return 无
         */
        virtual Monochrome_Screen_Error save_Char_To_Buffer(uint16_t x_offest, uint16_t y_offest, char chr);

        /**
         * 缓存图片到缓冲区指定位置
         * @param x_offest X坐标
         * @param y_offest y坐标
         * @param length 图片像素长度
         * @param width 图片像素宽度
         * @param IMG_Arr 图片数组
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error save_IMG_To_Buffer(uint16_t x_offest, uint16_t y_offest, uint16_t length, uint16_t width, const uint8_t *IMG_Arr);

    public:
        //默认构造函数
        Abs_Monochrome_Screen();

        /**
         *   输出一个字节数据
         *   @param chr 需要输出的字节数据
         *   @return IO_Stream_Error异常抛出
         */
        virtual IO_Stream_Error putchar(const char chr);

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
         * 设置字体族
         * @param font_Family 字体族数据组指针地址
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error set_Font_Family(uint8_t *font_Family);

        /**
         * 指定位置绘制像素点
         * @param x_offest X坐标
         * @param y_offest Y坐标
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_Pixel_at(uint16_t x_offest, uint16_t y_offest);

        /**
         * 向默认位置(0,0)绘制图形
         * @param length 图片像素长度
         * @param width 图片像素宽度
         * @param IMG_Arr 图片数组,每列8位点阵,字节垂直数据水平
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_IMG(uint16_t length, uint16_t width, const uint8_t *IMG_Arr);

        /**
         * 指定位置输出图片
         * @param x_offest X坐标
         * @param y_offest y坐标
         * @param length 图片像素长度
         * @param width 图片像素宽度
         * @param IMG_Arr 图片数组
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_IMG_at(uint16_t x_offest, uint16_t y_offest, uint16_t length, uint16_t width, const uint8_t *IMG_Arr) = 0;

        /**
         * 指定坐标打印字符
         * @param x_offest x坐标
         * @param y_offest y坐标
         * @param chr 输出字符
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error putchar_at(uint16_t x_offest, uint16_t y_offest, const char chr);

        /**
         * 指定位置格式化输出
         * @param x_offest x坐标
         * @param y_offest y坐标
         * @param lpFormatString 格式化输出字符串
         * @param ... 要打印的内容参数，支持如下:
         * @param -%d 输出整数
         * @param -%f 输出浮点数
         * @param -%s 输出字符串
         * @param -%c 输出字符
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error printf_at(uint16_t x_offest, uint16_t y_offest, const char *lpFormatString, ...);

        /**
         * 全页滚动屏幕
         * @param pos_count 滚动像素点计数
         * @return SSD1306_Error异常抛出
         */
        virtual Monochrome_Screen_Error screenRoll(uint16_t pos_count);

        /**
         * 获取建议的缓冲区大小
         * @param isFullBuffer 是否全页缓存
         * @return 建议的缓冲区长度(uint8_t)
         */
        virtual uint16_t get_Suggested_Buffer_Size(bool isFullBuffer);
    };

}

//单色屏资源区，字体及图像
namespace Monochrome_Screen_Resources
{
    //字节垂直，数据水平，标准ASCII字符6*8点阵，数据高位在点阵下方
    extern const uint8_t ASCII_0806[92][6];
}

#endif

#include "Abs_Monochrome_Screen.hpp"
#include "stddef.h"
#include "stdarg.h"

namespace cus
{
    //默认构造函数
    Abs_Monochrome_Screen::Abs_Monochrome_Screen()
    {
        //字体族相关设置
        set_Font_Family((const uint8_t *)Monochrome_Screen_Resources::ASCII_0806, 6, 8);
        //屏幕输出指针设置
        cursor_X = 0;
        cursor_Y = 0;
        //缓冲区设置
        isFullBuffer = false;
        buffer = NULL;
        //像素点相关设置
        Pixel_Color_Reverse = false;
        Pixel_Overlay = false;

        //未正确初始化
        isInit_already = false;

        // 后续内容需要在派生子类中实现
        // SCREEN_X 信息初始化;
        // SCREEN_Y 信息初始化;
    }

    /**
     * 设置字体族
     * @param Font_Family 字体族数据组指针地址
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::set_Font_Family(const uint8_t *Font_Family, uint8_t Font_PosLength, uint8_t Font_PosWidth)
    {

        //设置为默认值
        this->Font_Family = (const uint8_t *)Monochrome_Screen_Resources::ASCII_0806;
        this->Font_PosLength = 6;
        this->Font_PosWidth = 8;

        //返回功能未实现
        return MONOCHROME_SCREEN_ERROR_FUNCTION_UNREALIZED;
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

    /**
     * 缓存字符数据到轻量级缓冲区指定位置，将缓冲区当成一个二维字符矩阵使用
     * @param x_offest X坐标，需取整
     * @param y_offest Y坐标，需取整
     * @param data 需要缓存的数据
     * @return 无
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::save_Char_To_Buffer(uint16_t x_offest, uint16_t y_offest, char chr)
    {
        if (buffer != NULL)
        {
            //若是全页缓存则应该在draw_IMG_at中缓存
            if (isFullBuffer)
                return MONOCHROME_SCREEN_ERROR_NONE;
            else
                // 轻量级字符缓冲区，用于最小资源实现控制台缓冲翻页滚动
                // buffer用作字符缓存，每行存储SCREEN_X / Font_PosLength个字，变相二维数组
                // buffer [第0行][字1，字2，字3........] 共(SCREEN_X / Font_PosLength) 个字
                // buffer [第1行][字1，字2，字3........] 共(SCREEN_X / Font_PosLength) 个字
                // 以此类推，总共有（SCREEN_Y / Font_PosWidth） 行
                // buffer共可存储 (SCREEN_Y / Font_PosWidth) * (SCREEN_X / Font_PosLength) 个字
                // 前期有使用二维指针强转的方式实现buffer二维数组化使用，但是为了某些只支持C99标准的编译器妥协一下
                buffer[(y_offest / Font_PosWidth) * (SCREEN_X / Font_PosLength) + (x_offest / Font_PosLength)] = (uint8_t)chr;

            return MONOCHROME_SCREEN_ERROR_NONE;
        }
        //没有缓冲区
        return MONOCHROME_SCREEN_ERROR_NOBUFFER;
    }

    /**
     * 缓存图片到缓冲区指定位置
     * @param x_offest X坐标
     * @param y_offest y坐标
     * @param length 图片像素长度
     * @param width 图片像素宽度
     * @param IMG_Arr 图片数组
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::save_IMG_To_Buffer(uint16_t x_offest, uint16_t y_offest, uint16_t length, uint16_t width, const uint8_t *IMG_Arr)
    {
        //查询缓冲区状态
        if (isFullBuffer)
            return MONOCHROME_SCREEN_ERROR_FUNCTION_UNREALIZED;
        else
            return MONOCHROME_SCREEN_ERROR_NO_FULLBUFFER;
    }

    /**
     * 输出一个字节数据
     * @param chr 需要输出的字节数据
     * @return IO_Stream_Error异常抛出
     */
    IO_Stream_Error Abs_Monochrome_Screen::putchar(const char chr)
    {
        //管理内部指针以实现控制台输出，所有的输出及缓存工作全部嫁接交给putchar_at
        //更新cursor_X及cursor_Y时应该考虑到越界的问题

        //处理空字符
        if (chr == '\0')
            return IO_STREAM_ERROR_NONE;

        //处理换行符
        if (chr == '\n')
        {
            //正好指针越界，屏幕滚动字体宽度单位像素
            if (cursor_Y + Font_PosWidth >= SCREEN_Y)
            {
                if (screen_Roll(Font_PosWidth) != MONOCHROME_SCREEN_ERROR_NONE)
                    return IO_STREAM_ERROR_IMPLEMENT_LAYER_FAILD;
            }
            else
            {
                //记录换行符并换行
                save_Char_To_Buffer(cursor_X, cursor_Y, chr);
                cursor_Y += Font_PosWidth;
            }

            // x指针清零
            cursor_X = 0;
            return IO_STREAM_ERROR_NONE;
        }
        else
        {
            //检查当前行是否还可以容纳字符
            if (cursor_X + Font_PosLength < SCREEN_X)
            {
                //输出字符
                if (putchar_at(cursor_X, cursor_Y, chr) != MONOCHROME_SCREEN_ERROR_NONE)
                    return IO_STREAM_ERROR_IMPLEMENT_LAYER_FAILD;
                //缓存输出字符
                save_Char_To_Buffer(cursor_X, cursor_Y, chr);
                //更新指针，返回
                cursor_X += Font_PosLength;
                return IO_STREAM_ERROR_NONE;
            }
            else
            {
                //递归换行
                if (putchar('\n') != IO_STREAM_ERROR_NONE)
                    return IO_STREAM_ERROR_IMPLEMENT_LAYER_FAILD;

                //递归打印目标字符
                return putchar(chr);
            }
        }
    }

    /**
     * 指定坐标打印字符
     * @param x_offest x坐标
     * @param y_offest y坐标
     * @param chr 输出字符
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::putchar_at(uint16_t x_offest, uint16_t y_offest, const char chr)
    {
        //越界审查
        if (x_offest >= SCREEN_X)
            return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_X;
        if (y_offest >= SCREEN_Y)
            return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y;

        if (chr == '\0' || chr == '\n')
            return MONOCHROME_SCREEN_ERROR_NONE;

        //在指定位置输出字符图像
        return draw_IMG_at(cursor_X, cursor_Y, Font_PosLength, Font_PosWidth, &Font_Family[chr - ' ']);
    }

    /**
     * 向默认位置(0,0)绘制图形
     * @param length 图片像素长度
     * @param width 图片像素宽度
     * @param IMG_Arr 图片数组,每列8位点阵,字节垂直数据水平
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::draw_IMG(uint16_t length, uint16_t width, const uint8_t *IMG_Arr)
    {
        //检查是否在行首，否则换行再输出
        if (cursor_X != 0)
            if (putchar('\n') != IO_STREAM_ERROR_NONE)
                return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED;

        return draw_IMG_at(cursor_X, cursor_Y, length, width, IMG_Arr);
    }

    /**
     * 全页滚动屏幕
     * @param pos_count 滚动像素点计数
     * @return SSD1306_Error异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::screen_Roll(uint16_t pos_count)
    {
        if (buffer == NULL) //无缓冲区直接刷新后返回
            return clear();
        else if (isFullBuffer) //全缓冲平移像素点
        {
            //输出缓冲区内容并返回
            return draw_IMG_at(0, 0, SCREEN_X, SCREEN_Y, buffer);
        }
        else //无缓冲处理精简缓冲区
        {
            
        }
        return MONOCHROME_SCREEN_ERROR_NONE;
    }

    /**
     * 获取建议的缓冲区大小
     * @param isFullBuffer 是否全页缓存
     * @return 建议的缓冲区长度(uint8_t)
     */
    uint16_t Abs_Monochrome_Screen::get_Suggested_Buffer_Size(bool isFullBuffer)
    {
        if (isFullBuffer)
            return SCREEN_X * (SCREEN_Y / 8 + (SCREEN_Y % 8 == 0 ? 0 : 1)); //全页缓存
        else
            return (SCREEN_Y / Font_PosWidth) * (SCREEN_X / Font_PosLength); //轻量级缓存

        // 轻量级字符缓冲区，用于最小资源实现控制台缓冲翻页滚动
        // buffer用作字符缓存，每行存储SCREEN_X / Font_PosLength个字，变相二维数组
        // buffer [第0行][字1，字2，字3........] 共(SCREEN_X / Font_PosLength) 个字
        // buffer [第1行][字1，字2，字3........] 共(SCREEN_X / Font_PosLength) 个字
        // 以此类推，总共有（SCREEN_Y / Font_PosWidth） 行
        // buffer共可存储 (SCREEN_Y / Font_PosWidth) * (SCREEN_X / Font_PosLength) 个字
        // 前期有使用二维指针强转的方式实现buffer二维数组化使用，但是为了某些只支持C99标准的编译器妥协一下
    }

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
    Monochrome_Screen_Error Abs_Monochrome_Screen::printf_at(uint16_t x_offest, uint16_t y_offest, const char *lpFormatString, ...)
    {
        //初始化参数列表，指向不定参第一个参数
        va_list args;
        va_start(args, lpFormatString);

        //临时打印坐标记录
        uint8_t temp_cursor_X = x_offest;
        uint8_t temp_cursor_Y = y_offest;

        //轮询数据
        while (*lpFormatString != '\0')
        {
            //换行审查
            if (*lpFormatString == '\n') //检查换行符
            {
                temp_cursor_X = x_offest;
                temp_cursor_Y += Font_PosWidth;
                //页越界审查
                if (temp_cursor_Y >= SCREEN_Y)
                    return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y;
                lpFormatString++;
                continue;
            }
            //越界审查
            else if (temp_cursor_X + Font_PosLength >= SCREEN_X)
            {
                temp_cursor_X = x_offest;
                temp_cursor_Y += Font_PosWidth;
                if (temp_cursor_Y >= SCREEN_Y)
                    return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y;
                else
                    continue;
            }
            else if (*lpFormatString == '%') //筛查格式化标签
            {
                switch (*(lpFormatString + 1))
                {
                // //数型
                // case 'd':
                //     if (drawInteger_at(temp_cursor_X, temp_cursor_X, va_arg(args, int)) != MONOCHROME_SCREEN_ERROR_NONE)
                //         return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED;
                //     //跃迁格式化字符串指针
                //     lpFormatString += 2;
                //     //继续下一个检测
                //     continue;
                // case 'f':
                //     if (drawDecimal_at(temp_cursor_X, temp_cursor_X, va_arg(args, double)) != MONOCHROME_SCREEN_ERROR_NONE)
                //         return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED;
                //     //跃迁格式化字符串指针
                //     lpFormatString += 2;
                //     //继续下一个检测
                //     continue;
                // case 's':
                //     if (drawString_at(temp_cursor_X, temp_cursor_X, va_arg(args, const uint8_t *)) != MONOCHROME_SCREEN_ERROR_NONE)
                //         return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED;
                //     //跃迁格式化字符串指针
                //     lpFormatString += 2;
                //     //继续下一个检测
                //     continue;
                case 'c':
                    if (putchar_at(temp_cursor_X, temp_cursor_X, (uint8_t)va_arg(args, int)) != MONOCHROME_SCREEN_ERROR_NONE)
                        return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED;
                    //增加X指针
                    temp_cursor_X += Font_PosLength;
                    //边界换行审查
                    if (temp_cursor_X >= SCREEN_X)
                    {
                        if (temp_cursor_Y + Font_PosWidth >= SCREEN_Y)
                            return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y;
                        else
                            temp_cursor_Y += SCREEN_Y;
                        //既然已经发生换行，代表字符输出到了下一行0位
                        temp_cursor_X = x_offest + Font_PosLength;
                    }

                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    //继续下一个检测
                    continue;
                //不在检测范围内的标签
                default:
                    break;
                }
            }
            if (putchar_at(temp_cursor_X, temp_cursor_X, *(lpFormatString++)) != MONOCHROME_SCREEN_ERROR_NONE) //打印并更新指针
                return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED;
            temp_cursor_X += Font_PosLength;
        }
        return MONOCHROME_SCREEN_ERROR_NONE;
    }

    /**
     * 指定位置绘制像素点
     * @param x_offest X坐标
     * @param y_offest Y坐标
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error Abs_Monochrome_Screen::draw_Pixel_at(uint16_t x_offest, uint16_t y_offest)
    {
        return MONOCHROME_SCREEN_ERROR_FUNCTION_UNREALIZED;
    }

}

//单色屏资源区，字体及图像
namespace Monochrome_Screen_Resources
{
    //字节垂直，数据水平，标准ASCII字符6*8点阵，数据高位在点阵下方
    const uint8_t ASCII_0806[92][6] = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // sp
        {0x00, 0x00, 0x00, 0x2f, 0x00, 0x00}, // !
        {0x00, 0x00, 0x07, 0x00, 0x07, 0x00}, // "
        {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
        {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
        {0x00, 0x62, 0x64, 0x08, 0x13, 0x23}, // %
        {0x00, 0x36, 0x49, 0x55, 0x22, 0x50}, // &
        {0x00, 0x00, 0x05, 0x03, 0x00, 0x00}, // '
        {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00}, // (
        {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00}, // )
        {0x00, 0x14, 0x08, 0x3E, 0x08, 0x14}, // *
        {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08}, // +
        {0x00, 0x00, 0x00, 0xA0, 0x60, 0x00}, // ,
        {0x00, 0x08, 0x08, 0x08, 0x08, 0x08}, // -
        {0x00, 0x00, 0x60, 0x60, 0x00, 0x00}, // .
        {0x00, 0x20, 0x10, 0x08, 0x04, 0x02}, // /
        {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
        {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
        {0x00, 0x42, 0x61, 0x51, 0x49, 0x46}, // 2
        {0x00, 0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
        {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
        {0x00, 0x27, 0x45, 0x45, 0x45, 0x39}, // 5
        {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
        {0x00, 0x01, 0x71, 0x09, 0x05, 0x03}, // 7
        {0x00, 0x36, 0x49, 0x49, 0x49, 0x36}, // 8
        {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
        {0x00, 0x00, 0x36, 0x36, 0x00, 0x00}, // :
        {0x00, 0x00, 0x56, 0x36, 0x00, 0x00}, // ;
        {0x00, 0x08, 0x14, 0x22, 0x41, 0x00}, // <
        {0x00, 0x14, 0x14, 0x14, 0x14, 0x14}, // =
        {0x00, 0x00, 0x41, 0x22, 0x14, 0x08}, // >
        {0x00, 0x02, 0x01, 0x51, 0x09, 0x06}, // ?
        {0x00, 0x32, 0x49, 0x59, 0x51, 0x3E}, // @
        {0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C}, // A
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36}, // B
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22}, // C
        {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41}, // E
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01}, // F
        {0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
        {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
        {0x00, 0x00, 0x41, 0x7F, 0x41, 0x00}, // I
        {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01}, // J
        {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41}, // K
        {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40}, // L
        {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
        {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06}, // P
        {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
        {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46}, // R
        {0x00, 0x46, 0x49, 0x49, 0x49, 0x31}, // S
        {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01}, // T
        {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
        {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
        {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
        {0x00, 0x63, 0x14, 0x08, 0x14, 0x63}, // X
        {0x00, 0x07, 0x08, 0x70, 0x08, 0x07}, // Y
        {0x00, 0x61, 0x51, 0x49, 0x45, 0x43}, // Z
        {0x00, 0x00, 0x7F, 0x41, 0x41, 0x00}, // [
        {0x01, 0x06, 0x08, 0x30, 0x40, 0x00}, // ‘\’
        {0x00, 0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
        {0x00, 0x04, 0x02, 0x01, 0x02, 0x04}, // ^
        {0x00, 0x40, 0x40, 0x40, 0x40, 0x40}, // _
        {0x00, 0x00, 0x01, 0x02, 0x04, 0x00}, // '
        {0x00, 0x20, 0x54, 0x54, 0x54, 0x78}, // a
        {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38}, // b
        {0x00, 0x38, 0x44, 0x44, 0x44, 0x20}, // c
        {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F}, // d
        {0x00, 0x38, 0x54, 0x54, 0x54, 0x18}, // e
        {0x00, 0x08, 0x7E, 0x09, 0x01, 0x02}, // f
        {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C}, // g
        {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78}, // h
        {0x00, 0x00, 0x44, 0x7D, 0x40, 0x00}, // i
        {0x00, 0x40, 0x80, 0x84, 0x7D, 0x00}, // j
        {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00}, // k
        {0x00, 0x00, 0x41, 0x7F, 0x40, 0x00}, // l
        {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78}, // m
        {0x00, 0x7C, 0x08, 0x04, 0x04, 0x78}, // n
        {0x00, 0x38, 0x44, 0x44, 0x44, 0x38}, // o
        {0x00, 0xFC, 0x24, 0x24, 0x24, 0x18}, // p
        {0x00, 0x18, 0x24, 0x24, 0x18, 0xFC}, // q
        {0x00, 0x7C, 0x08, 0x04, 0x04, 0x08}, // r
        {0x00, 0x48, 0x54, 0x54, 0x54, 0x20}, // s
        {0x00, 0x04, 0x3F, 0x44, 0x44, 0x20}, // t
        {0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
        {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
        {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
        {0x00, 0x44, 0x28, 0x10, 0x28, 0x44}, // x
        {0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C}, // y
        {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44}, // z
        {0x14, 0x14, 0x14, 0x14, 0x14, 0x14}, // horiz lines
    };
}
#include "SSD1306.hpp"
#include "stddef.h"
#include "stdarg.h"

//宏定义
#define SSD1306_CMD 0            // SSD1306控制帧标识
#define SSD1306_DATA 1           // SSD1306数据帧标识
#define SSD1306_ADDR 0X3C        // SSD1306地址
#define IIC_SPEED_THRESHOLD 5    //信号稳定时延&刷新时延
#define DECIMAL_HOLD_MAX 5       //小数点精度
#define ASCII_DATA_LENGTH 6      //默认字符集数据长度
#define ASCII_DATA_POS_WIDTH 8   //默认字符集数据像素宽度
#define SSD1306_PAGE_POS_WIDTH 8 //页像素宽度

// ASCII字符集6*8点阵
const static uint8_t ASCII_0806[92][6] = {
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
namespace cus
{
    //构造函数
    SSD1306::SSD1306()
    {
        isInit_already = false;
    }
    /**
     * 带参构造函数
     * @param Abs_IIC_Master实例
     */
    SSD1306::SSD1306(Abs_IIC_Master &IIC_Wire, uint8_t ScreenX, uint8_t ScreenY)
    {
        construct(IIC_Wire, ScreenX, ScreenY);
    }
    /**
     * 构造&初始化
     * @param Abs_IIC_Master实例
     * @return SSD1306实例
     */
    SSD1306 &SSD1306::construct(Abs_IIC_Master &IIC_Wire, uint8_t ScreenX, uint8_t ScreenY)
    {
        //相关参数初始化
        this->IIC_Wire = &IIC_Wire;
        this->SCREEN_X = ScreenX;
        this->SCREEN_Y = ScreenY;
        this->SCREEN_PAGE = ScreenY / SSD1306_PAGE_POS_WIDTH;
        this->SCREEN_PAGE_ASCII_CHAR_MAX = ScreenX / ASCII_DATA_LENGTH;
        this->cursor_X = 0;
        this->cursor_Y = 0;
        this->buffer = NULL;
        this->isFullBuffer = false;

        // IIC相关设置
        this->IIC_Wire->focus_on(SSD1306_ADDR);

        //初始化完成
        isInit_already = true;

        //初始化屏幕参数&唤醒
        for (int count = 0; count < 200; count++)
            if (initScreen() == SSD1306_ERROR_NONE)
                break;

        return *this;
    }

    /**
     *   颠倒颜色
     *   @param YesOrNo 是否颠倒颜色
     */
    SSD1306_Error SSD1306::colorTurn(bool YesOrNo)
    {
        if (isInit_already)
        {

            if (IIC_Wire->write(0x00) != IIC_ERROR_NONE)
                return SSD1306_ERROR_WRITE_FAILED;
            if (IIC_Wire->write(YesOrNo ? 0xA7 : 0xA6) != IIC_ERROR_NONE)
                return SSD1306_ERROR_WRITE_FAILED;
            IIC_Wire->end();
            return SSD1306_ERROR_NONE;
        }
        else
            return SSD1306_ERROR_UNINITED;
    }
    /**
     *   设置打印起始笔刷位置
     *   @param x_offest x坐标
     *   @param page_offest 页面坐标
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::setDisplayBrush(uint8_t x_offest, uint8_t page_offest)
    {
        //越界审查
        if (x_offest >= SCREEN_X)
            return SSD1306_ERROR_OUT_OF_RANGE_X;
        if (page_offest >= SCREEN_PAGE)
            return SSD1306_ERROR_OUT_OF_RANGE_Y;
        //设置像素点位置命令
        if (IIC_Wire->write(0x00) == IIC_ERROR_NONE)
            if (IIC_Wire->write(0xb0 + page_offest) == IIC_ERROR_NONE)
                if (IIC_Wire->write(((x_offest & 0xf0) >> 4) | 0x10) == IIC_ERROR_NONE)
                    if (IIC_Wire->write((x_offest & 0x0f)) == IIC_ERROR_NONE)
                    {
                        IIC_Wire->end();
                        return SSD1306_ERROR_NONE;
                    }
        return SSD1306_ERROR_WRITE_FAILED;
    }
    /**
     *   清理屏幕
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::clear()
    {
        //缓冲区清除
        clearBuffer();
        //正确初始化检查
        if (isInit_already == false)
            return SSD1306_ERROR_UNINITED;
        //清理屏幕
        for (uint8_t pageCount = 0; pageCount < SCREEN_PAGE; pageCount++)
        {
            //设置坐标
            if (setDisplayBrush(0, pageCount) != SSD1306_ERROR_NONE)
                return SSD1306_ERROR_BRUSH_SET_FAILED;
            //刷新行数
            if (IIC_Wire->write(0x40) != IIC_ERROR_NONE) //写入DATA
                return SSD1306_ERROR_WRITE_FAILED;
            for (uint8_t offest_x = 0; offest_x < SCREEN_X; offest_x++)
                if (IIC_Wire->write(0x00) != IIC_ERROR_NONE) //循环写入空像素
                    return SSD1306_ERROR_WRITE_FAILED;
            IIC_Wire->end();
        }
        //回退数屏幕指针
        cursor_X = 0;
        cursor_Y = 0;
        //返回
        return SSD1306_ERROR_NONE;
    }
    /**
     *   设置屏幕全页缓存
     *   @param lpBuffer 缓存数据指针
     *   @param size 缓冲区长度
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::setFullBuffer(uint8_t *lpBuffer, uint16_t size)
    {
        if (isInit_already)
        {
            if (size >= SCREEN_PAGE * SCREEN_X)
            {
                buffer = lpBuffer;
                isFullBuffer = true;
                clear();
                return SSD1306_ERROR_NONE;
            }
            return SSD1306_ERROR_BUFFER_SIZE_INCORRECT;
        }
        return SSD1306_ERROR_UNINITED;
    }
    /**
     *   设置屏幕轻量级缓存,仅支持字符缓存
     *   @param lpBuffer 缓存数据指针
     *   @param size 缓冲区长度
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::setLightBuffer(uint8_t *lpBuffer, uint16_t size)
    {
        if (isInit_already)
        {
            if (size >= SCREEN_PAGE * SCREEN_PAGE_ASCII_CHAR_MAX)
            {
                buffer = lpBuffer;
                isFullBuffer = false;
                clear();
                return SSD1306_ERROR_NONE;
            }
            return SSD1306_ERROR_BUFFER_SIZE_INCORRECT;
        }
        return SSD1306_ERROR_UNINITED;
    }
    /**
     * 缓冲区清理
     * @return 无
     */
    void SSD1306::clearBuffer()
    {
        //回退指针
        cursor_X = 0;
        cursor_Y = 0;
        if (buffer == NULL)
            return;
        //清除缓冲区
        if (isFullBuffer)
            for (int count = SCREEN_X * SCREEN_PAGE - 1; count >= 0; count--)
                buffer[count] = 0x00;
        else
            for (int count = SCREEN_PAGE_ASCII_CHAR_MAX * SCREEN_PAGE - 1; count >= 0; count--)
                buffer[count] = ' ';
    }
    /**
     *   使用类内信息初始化屏幕
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::initScreen()
    {
        //初始化全程所需命令行
        const uint8_t initCmdLine[29] = {
            0x00, //表明输入cmd
            0xAE, //--turn off oled panel
            0x00, //---set low column address
            0x10, //---set high column address
            0x40, //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
            0x81, //--set contrast control register
            0xCF, // Set SEG Output Current Brightness
            0xA1, //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
            0xC8, // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
            0xA6, //--set normal display
            0xA8, //--set multiplex ratio(1 to 64)
            0x3f, //--1/64 duty
            0xD3, //-set display offset Shift Mapping RAM Counter (0x00~0x3F)
            0x00, //-not offset
            0xd5, //--set display clock divide ratio/oscillator frequency
            0x80, //--set divide ratio, Set Clock as 100 Frames/Sec
            0xD9, //--set pre-charge period
            0xF1, // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
            0xDA, //--set com pins hardware configuration
            0x12,
            0xDB, //--set vcomh
            0x40, // Set VCOM Deselect Level
            0x20, //-Set Page Addressing Mode (0x00/0x01/0x02)
            0x02, //
            0x8D, //--set Charge Pump enable/disable
            0x14, //--set(0x10) disable
            0xA4, // Disable Entire Display On (0xa4/0xa5)
            0xA6, // Disable Inverse Display On (0xa6/a7)
            0xAF, // display ON
        };
        //写入上述初始化指令
        for (int count = 0; count < 29; count++)
        {
            //循环写入
            if (IIC_Wire->write(initCmdLine[count]) != IIC_ERROR_NONE)
                return SSD1306_ERROR_WRITE_FAILED;
        }
        //清理屏幕
        if (clear() != SSD1306_ERROR_NONE)
            return SSD1306_ERROR_CLEAR_FAILED;
        //发送初始化信息
        if (printf(0, 0, "Screen Init OK!!\n\nPowered by:\n\nAbstract BSP Lib.\nFrom LuChiChick.\n\n>2022\\4\\17 Ver.") != SSD1306_ERROR_NONE)
            return SSD1306_ERROR_PRINT_FAILED;
        //返回
        return SSD1306_ERROR_NONE;
    }
    /**
     *   向屏幕输出一个字符
     *   @param chr 需要打印的字符
     *   @return IO_Stream_Error异常抛出
     */
    IO_Stream_Error SSD1306::putchar(const char chr)
    {
        if (isInit_already)
        {
            //处理换行符
            if (chr == '\n')
            {
                drawCharToBuffer(chr);
                //正好翻页时
                if (cursor_Y >= SCREEN_Y)
                {
                    cursor_X = 0;
                    if (screenRoll(ASCII_DATA_POS_WIDTH) != SSD1306_ERROR_NONE)
                        return IO_STREAM_ERROR_PUTCHAR_FAILED;
                    else
                        return IO_STREAM_ERROR_NONE;
                }
                else
                    cursor_Y += ASCII_DATA_POS_WIDTH;
                cursor_X = 0;
                return IO_STREAM_ERROR_NONE;
            }
            //处理页溢出
            if (cursor_Y >= SCREEN_Y)
            {
                if (screenRoll(ASCII_DATA_POS_WIDTH) != SSD1306_ERROR_NONE)
                    return IO_STREAM_ERROR_PUTCHAR_FAILED;
                cursor_X = 0;
            }
            //处理行溢出
            if (cursor_X >= SCREEN_PAGE_ASCII_CHAR_MAX * ASCII_DATA_LENGTH)
            {
                if (cursor_Y + ASCII_DATA_POS_WIDTH >= SCREEN_Y)
                {
                    if (screenRoll(ASCII_DATA_POS_WIDTH) != SSD1306_ERROR_NONE)
                        return IO_STREAM_ERROR_PUTCHAR_FAILED;
                }
                else
                    cursor_Y += ASCII_DATA_POS_WIDTH;
                cursor_X = 0;
            }

            //打印字符
            if (putchar(cursor_X, cursor_Y, chr) != SSD1306_ERROR_NONE)
                return IO_STREAM_ERROR_PUTCHAR_FAILED;
            cursor_X += ASCII_DATA_LENGTH;
            return IO_STREAM_ERROR_NONE;
        }
        return IO_STREAM_ERROR_PUTCHAR_FAILED;
    }
    /**
     *   指定坐标打印字符
     *   @param x_offest x坐标
     *   @param y_offest y坐标
     *   @param chr 输出字符
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::putchar(uint8_t x_offest, uint8_t y_offest, const char chr)
    {
        //正确初始化检查
        if (isInit_already == false)
            return SSD1306_ERROR_UNINITED;
        //排查是否越界
        if (y_offest >= SCREEN_Y)
            return SSD1306_ERROR_OUT_OF_RANGE_Y;
        if (x_offest >= SCREEN_X)
            return SSD1306_ERROR_OUT_OF_RANGE_X;
        //缓存字符
        drawCharToBuffer(x_offest, y_offest, chr);
        //计算是否需要偏移像素点分两行打印(即坐标是否能被PagePos整除)
        uint8_t pos_offest = y_offest % SSD1306_PAGE_POS_WIDTH;
        //打印上半部分，若能整除就结束
        if (setDisplayBrush(x_offest, y_offest / SSD1306_PAGE_POS_WIDTH) != SSD1306_ERROR_NONE)
            return SSD1306_ERROR_BRUSH_SET_FAILED;
        if (IIC_Wire->write(0x40) != IIC_ERROR_NONE) //表明写入data
            return SSD1306_ERROR_WRITE_FAILED;
        for (uint8_t count = 0; count < ((x_offest + ASCII_DATA_LENGTH) <= SCREEN_X ? ASCII_DATA_LENGTH : SCREEN_X); count++)
        {
            if (IIC_Wire->write((ASCII_0806[chr - ' '][count]) << pos_offest) != IIC_ERROR_NONE) //偏移pos_offest单位
                return SSD1306_ERROR_WRITE_FAILED;
        }
        IIC_Wire->end();

        //判断是否需要打印下半行
        if (pos_offest > 0)
        {
            //处理临界打印
            if ((y_offest / SSD1306_PAGE_POS_WIDTH) + 1 == SCREEN_PAGE)
                return SSD1306_ERROR_NONE;

            if (setDisplayBrush(x_offest, (y_offest / SSD1306_PAGE_POS_WIDTH) + 1) != SSD1306_ERROR_NONE)
                return SSD1306_ERROR_BRUSH_SET_FAILED;
            if (IIC_Wire->write(0x40) != IIC_ERROR_NONE) //表明写入data
                return SSD1306_ERROR_WRITE_FAILED;
            for (uint16_t count = 0; count < ((x_offest + ASCII_DATA_LENGTH) <= SCREEN_X ? ASCII_DATA_LENGTH : SCREEN_X); count++)
            {
                if (IIC_Wire->write((ASCII_0806[chr - ' '][count]) >> (SSD1306_PAGE_POS_WIDTH - pos_offest)) != IIC_ERROR_NONE) //反向偏移
                    return SSD1306_ERROR_WRITE_FAILED;
            }
            IIC_Wire->end();
        }
        return SSD1306_ERROR_NONE;
    }
    /**
     * 绘制字符到缓存
     * @param chr 缓存的字符
     */
    void SSD1306::drawCharToBuffer(char chr)
    {
        drawCharToBuffer(cursor_X, cursor_Y, chr);
    }
    /**
     * 绘制字符到缓存
     * @param chr 缓存的字符
     * @param x_offest X坐标
     * @param y_offest Y坐标
     */
    void SSD1306::drawCharToBuffer(uint8_t x_offest, uint8_t y_offest, char chr)
    {
        //无缓存
        if (buffer == NULL)
            return;
        //全页缓存
        if (isFullBuffer)
        {
            //计算是否需要偏移像素点分两行打印(即坐标是否能被PagePos整除)
            uint8_t pos_offest = y_offest % SSD1306_PAGE_POS_WIDTH;
            uint16_t targetDir = (y_offest / SSD1306_PAGE_POS_WIDTH) * SCREEN_X + x_offest;
            //像素点或运算
            for (int count = ((x_offest + ASCII_DATA_LENGTH) < SCREEN_X ? ASCII_DATA_LENGTH : SCREEN_X - x_offest) - 1; count >= 0; count--)
                buffer[targetDir + count] |= (chr == '\n' ? 0x00 : (ASCII_0806[chr - ' '][count]) << pos_offest);

            //处理偏移
            if ((pos_offest > 0) && ((targetDir += SCREEN_X) <= SCREEN_X * SCREEN_PAGE))
                for (int count = ((x_offest + ASCII_DATA_LENGTH) < SCREEN_X ? ASCII_DATA_LENGTH : SCREEN_X - x_offest) - 1; count >= 0; count--)
                    buffer[targetDir + count] |= (chr == '\n' ? 0x00 : (ASCII_0806[chr - ' '][count]) >> pos_offest);
        }
        //轻量缓存
        else
        {
            if ((cursor_X < SCREEN_X) && (cursor_Y < SCREEN_Y))
                buffer[(cursor_Y / ASCII_DATA_POS_WIDTH) * SCREEN_PAGE_ASCII_CHAR_MAX + (cursor_X / ASCII_DATA_LENGTH)] = chr;
        }
    }

    /**
     *   递归打印整数
     *   @param num 输出整数
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::drawInteger(uint8_t &cursor_X, uint8_t &cursor_Y, int num)
    {
        //处理负值
        if (num < 0)
        {
            //处理负值后正式开始打印
            if (putchar(cursor_X, cursor_Y, '-') != SSD1306_ERROR_NONE)
                return SSD1306_ERROR_PRINT_FAILED;
            //更新计数
            cursor_X += ASCII_DATA_LENGTH;
            num = -num;
        }
        //处理常规值
        if (num >= 0 && num < 10) //递归出口
        {
            if (putchar(cursor_X, cursor_Y, '0' + num) != SSD1306_ERROR_NONE)
                return SSD1306_ERROR_PRINT_FAILED;
            cursor_X += ASCII_DATA_LENGTH;
            return SSD1306_ERROR_NONE;
        }
        else
        {
            //整除10为打印前一位，取余为打印自身
            if (drawInteger(cursor_X, cursor_Y, num / 10) == SSD1306_ERROR_NONE)
                return drawInteger(cursor_X, cursor_Y, num % 10);
            else
                return SSD1306_ERROR_PRINT_FAILED;
        }
        return SSD1306_ERROR_NONE;
    }

    /**
     *   指定坐标打印浮点数,仅用于内部，带有坐标更新
     *   @param x_offest x坐标
     *   @param y_offest y坐标
     *   @param num 输出浮点数
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::drawDecimal(uint8_t &x_offest, uint8_t &y_offest, double num)
    {
        //处理原来的整数部分
        if (drawInteger(x_offest, y_offest, (int)(num * 1.0)) != SSD1306_ERROR_NONE)
            return SSD1306_ERROR_DRAW_INT_FAILED;
        //检查行溢出
        if (x_offest + ASCII_DATA_LENGTH >= SCREEN_X)
            return SSD1306_ERROR_NONE;
        //整理数字
        num = num - (int)num;
        if (putchar(x_offest, y_offest, '.') != SSD1306_ERROR_NONE)
            return SSD1306_ERROR_PRINT_FAILED;
        //检查行溢出
        if (x_offest + ASCII_DATA_LENGTH >= SCREEN_X)
            return SSD1306_ERROR_NONE;
        //更新坐标
        x_offest += ASCII_DATA_LENGTH;
        //扩大倍数转将小数部分换成整数来解决问题
        for (int count = 0; count < DECIMAL_HOLD_MAX; count++)
            num *= 10;
        //忽略小数部分多余的0
        while ((int)num % 10 == 0 && (int)num != 0)
            num = (int)num / 10;
        //处理正负号
        if (num < 0)
            num = -num;
        //调用整数绘制
        return drawInteger(x_offest, y_offest, (int)num);
    }
    /**
     *   指定坐标字符串,仅用于内部，带有坐标更新
     *   @param cursor_X x指针
     *   @param cursor_Y y指针
     *   @param string 输出uint8_t字符串
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::drawString(uint8_t &cursor_X, uint8_t &cursor_Y, const uint8_t *string)
    {
        //轮询数据
        while (*string != '\0')
        {
            //行越界审查
            if (cursor_X + ASCII_DATA_LENGTH >= SCREEN_X)
                return SSD1306_ERROR_NONE;
            //检查特殊字符
            if (*string == '\n')
            {
                if (putchar(cursor_X, cursor_Y, '\\') != SSD1306_ERROR_NONE) //打印并更新指针
                    return SSD1306_ERROR_PRINT_FAILED;
                cursor_X += ASCII_DATA_LENGTH;
                if (putchar(cursor_X, cursor_Y, 'n') != SSD1306_ERROR_NONE) //打印并更新指针
                    return SSD1306_ERROR_PRINT_FAILED;
                cursor_X += ASCII_DATA_LENGTH;
                string++;
                continue;
            }

            if (putchar(cursor_X, cursor_Y, *(string++)) != SSD1306_ERROR_NONE) //打印并更新指针
                return SSD1306_ERROR_PRINT_FAILED;
            cursor_X += ASCII_DATA_LENGTH;
        }
        return SSD1306_ERROR_NONE;
    }

    /**
     *   指定位置格式化输出
     *   @param x_offest x坐标
     *   @param y_offest y坐标
     *   @param lpFormatString 格式化输出字符串
     *   @param ... 要打印的内容参数，支持如下:
     *   @param -%d 输出整数
     *   @param -%f 输出浮点数
     *   @param -%s 输出字符串
     *   @param -%c 输出字符
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::printf(uint8_t x_offest, uint8_t y_offest, const char *lpFormatString, ...)
    {
        //初始化参数列表，指向不定参第一个参数
        va_list args;
        va_start(args, lpFormatString);
        //打印坐标记录
        uint8_t cursorX = x_offest;
        uint8_t cursorY = y_offest;
        //轮询数据
        while (*lpFormatString != '\0')
        {
            //行越界审查
            if (cursorX + ASCII_DATA_LENGTH >= SCREEN_X)
                return SSD1306_ERROR_NONE;
            //检查换行符
            if (*lpFormatString == '\n')
            {
                cursorX = x_offest;
                cursorY += SSD1306_PAGE_POS_WIDTH;
                //页越界审查
                if (cursorY >= SCREEN_PAGE * SSD1306_PAGE_POS_WIDTH)
                    return SSD1306_ERROR_NONE;
                lpFormatString++;
                continue;
            }
            //筛查格式化标签
            if (*lpFormatString == '%')
            {
                switch (*(lpFormatString + 1))
                {
                //数型
                case 'd':
                    if (drawInteger(cursorX, cursorY, va_arg(args, int)) != SSD1306_ERROR_NONE)
                        return SSD1306_ERROR_DRAW_INT_FAILED;
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    //继续下一个检测
                    continue;
                case 'f':
                    if (drawDecimal(cursorX, cursorY, va_arg(args, double)) != SSD1306_ERROR_NONE)
                        return SSD1306_ERROR_DRAW_DEC_FAILED;
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    //继续下一个检测
                    continue;
                case 's':
                    if (drawString(cursorX, cursorY, va_arg(args, const uint8_t *)) != SSD1306_ERROR_NONE)
                        return SSD1306_ERROR_DRAW_DEC_FAILED;
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    //继续下一个检测
                    continue;
                case 'c':
                    if (putchar(cursorX, cursorY, (uint8_t)va_arg(args, int)) != SSD1306_ERROR_NONE)
                        return SSD1306_ERROR_PRINT_FAILED;
                    cursorX += ASCII_DATA_LENGTH;
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    //继续下一个检测
                    continue;
                //不在检测范围内的标签
                default:
                    break;
                }
            }
            if (putchar(cursorX, cursorY, *(lpFormatString++)) != SSD1306_ERROR_NONE) //打印并更新指针
                return SSD1306_ERROR_PRINT_FAILED;
            cursorX += ASCII_DATA_LENGTH;
        }
        return SSD1306_ERROR_NONE;
    }
    /**
     *   全页滚动屏幕
     *   @param pos_count 滚动像素点计数
     *   @return SSD1306_Error异常抛出
     */
    SSD1306_Error SSD1306::screenRoll(uint8_t pos_count)
    {
        //无缓冲区或翻页溢出
        if (buffer == NULL || pos_count >= SCREEN_Y)
        {
            clear();
            return SSD1306_ERROR_NONE;
        }
        //全页缓存
        if (isFullBuffer)
        {
            //计算是否需要偏移像素点分两行处理(即坐标是否能被PagePos整除)
            uint8_t pos_offest = pos_count % SSD1306_PAGE_POS_WIDTH;

            //滚动结束整页数
            int endPage = SCREEN_PAGE - pos_count / SSD1306_PAGE_POS_WIDTH;
            //页遍历
            for (int page_Count = 0; page_Count < endPage; page_Count++)
            {
                int target_begin = page_Count * SCREEN_X;
                int target_offest = (page_Count + pos_count / SSD1306_PAGE_POS_WIDTH) * SCREEN_X;
                for (int count = 0; count < SCREEN_X; count++)
                {
                    //处理像素点位移
                    if (pos_offest > 0)
                        buffer[target_begin + count] = buffer[target_begin + count] << pos_offest | buffer[target_offest + count] >> pos_offest;
                    else
                        buffer[target_begin + count] = buffer[target_offest + count];
                }
            }
            //后续无用字节处理
            //页遍历
            for (int page_Count = endPage; page_Count < SCREEN_PAGE; page_Count++)
            {
                int target_begin = page_Count * SCREEN_X;
                //滚动结束非整数页处理
                for (int count = 0; count < SCREEN_X; count++)
                    buffer[target_begin + count] = 0x00;
            }
            drawIMG(0, 0, SCREEN_X, SCREEN_Y, buffer);
        }
        //轻量级缓存
        else
        {
            for (int count = pos_count / ASCII_DATA_POS_WIDTH; count > 0; count--)
            {
                bool changed;
                int target_begin;                                                       //字符是否发生交换，用来节省刷新时间
                for (int writtenPage = 0; writtenPage < SCREEN_PAGE - 1; writtenPage++) //页遍历
                {
                    //目标字符起始
                    target_begin = writtenPage * SCREEN_PAGE_ASCII_CHAR_MAX;
                    //比对处理
                    for (int charFocus = 0; charFocus < SCREEN_PAGE_ASCII_CHAR_MAX; charFocus++) //行遍历
                    {
                        changed = false;
                        //数据不同时
                        if (buffer[target_begin + charFocus] != buffer[target_begin + SCREEN_PAGE_ASCII_CHAR_MAX + charFocus])
                        {
                            //变换并标记changed
                            buffer[target_begin + charFocus] = buffer[target_begin + SCREEN_PAGE_ASCII_CHAR_MAX + charFocus];
                            changed = true;
                        }
                        //处理换行符
                        if (buffer[target_begin + charFocus] == '\n')
                        {
                            int target_X = charFocus * ASCII_DATA_LENGTH;
                            //清除冗余数据避免风险
                            for (int count = charFocus + 1; count < SCREEN_PAGE_ASCII_CHAR_MAX; count++)
                                buffer[target_begin + count] = 0x00;
                            //设置光标位置
                            if (setDisplayBrush(target_X, writtenPage) != SSD1306_ERROR_NONE)
                                return SSD1306_ERROR_BRUSH_SET_FAILED;
                            if (IIC_Wire->write(0x40) != IIC_ERROR_NONE) // data命令
                                return SSD1306_ERROR_WRITE_FAILED;
                            //写入空白
                            for (int posCount = 0; posCount < SCREEN_X - target_X; posCount++)
                            {
                                if (IIC_Wire->write(0x00) != IIC_ERROR_NONE) //写入空白
                                    return SSD1306_ERROR_WRITE_FAILED;
                            }
                            IIC_Wire->end(); //结束通信
                            break;           //跳转到下一行
                        }
                        if (changed)
                        {
                            //刷新字符
                            if (putchar(charFocus * ASCII_DATA_LENGTH, writtenPage * SSD1306_PAGE_POS_WIDTH, buffer[target_begin + charFocus]) != SSD1306_ERROR_NONE)
                                return SSD1306_ERROR_PRINT_FAILED;
                        }
                    }
                }
            }
            //处理最后一行，因为会复写，所以不用清除缓冲区内多余的垃圾数据
            //设置光标位置
            if (setDisplayBrush(0, SCREEN_PAGE - 1) != SSD1306_ERROR_NONE)
                return SSD1306_ERROR_BRUSH_SET_FAILED;
            if (IIC_Wire->write(0x40) != IIC_ERROR_NONE) // data命令
                return SSD1306_ERROR_WRITE_FAILED;
            for (int posCount = 0; posCount < SCREEN_X; posCount++)
            {
                if (IIC_Wire->write(0x00) != IIC_ERROR_NONE) //写入空白
                    return SSD1306_ERROR_WRITE_FAILED;
            }
            IIC_Wire->end(); //结束通信
        }
        //重置X光标
        cursor_X = 0;
        //重置Y光标区
        cursor_Y = SCREEN_Y - pos_count;
        return SSD1306_ERROR_NONE;
    }
    /**
     *   指定位置输出图片
     *   @param x_offest X坐标
     *   @param y_offest y坐标
     *   @param sizex 图片长度
     *   @param sizey 图片宽度
     */
    SSD1306_Error SSD1306::drawIMG(uint8_t x_offest, uint8_t y_offest, uint8_t sizex, uint8_t sizey, const uint8_t *IMG_Arr)
    {
        //计算是否需要偏移像素点分两行打印(即坐标是否能被PagePos整除)
        int pos_offest = y_offest % SSD1306_PAGE_POS_WIDTH;
        //确定打印宽度
        int print_Width = (sizex + x_offest >= SCREEN_X) ? SCREEN_X - x_offest : sizex;
        //滚动页面
        for (int writtenPageCount = 0; writtenPageCount < sizey / SSD1306_PAGE_POS_WIDTH; writtenPageCount++)
        {
            //越界审查
            if (writtenPageCount + y_offest / SSD1306_PAGE_POS_WIDTH >= SCREEN_PAGE)
            {
                //有缓冲区更新指针
                if (isFullBuffer)
                {
                    cursor_Y = SCREEN_Y;
                    cursor_X = 0;
                }
                return SSD1306_ERROR_NONE;
            }
            //设置坐标
            if (setDisplayBrush(x_offest, writtenPageCount + y_offest / SSD1306_PAGE_POS_WIDTH) != SSD1306_ERROR_NONE)
                return SSD1306_ERROR_BRUSH_SET_FAILED;
            if (IIC_Wire->write(0x40) != IIC_ERROR_NONE) //表明写入data
                return SSD1306_ERROR_WRITE_FAILED;

            //打印每行的数据
            int target_begin = writtenPageCount * sizex;
            int buffer_target_begin = writtenPageCount * SCREEN_X;
            for (int count = 0; count < print_Width; count++)
            {
                //处理第一行
                if (writtenPageCount == 0)
                {
                    if (IIC_Wire->write((IMG_Arr[target_begin + count]) << pos_offest) != IIC_ERROR_NONE) //偏移pos_offest单位
                        return SSD1306_ERROR_WRITE_FAILED;
                    //缓冲区缓存
                    if (isFullBuffer)
                        buffer[buffer_target_begin + count] = (IMG_Arr[target_begin + count]) << pos_offest;
                    //这个continue相当重要！！！！！！
                    continue;
                }
                //打印常规行,偏移8-pos_offest单位并承接上一行的数据
                if (IIC_Wire->write((IMG_Arr[target_begin + count]) << pos_offest | (IMG_Arr[target_begin - sizex + count]) >> (SSD1306_PAGE_POS_WIDTH - pos_offest)) != IIC_ERROR_NONE)
                    return SSD1306_ERROR_WRITE_FAILED;
                //缓冲区缓存
                if (isFullBuffer)
                    buffer[buffer_target_begin + count] = (IMG_Arr[target_begin + count]) << pos_offest | (IMG_Arr[target_begin - sizex + count]) >> (SSD1306_PAGE_POS_WIDTH - pos_offest);
            }
            //结束一行的打印
            IIC_Wire->end();
            //处理最后一行，需要存在偏移，且不能越界
            if (pos_offest > 0 && writtenPageCount == (sizey / SSD1306_PAGE_POS_WIDTH) - 1 && writtenPageCount + 1 + y_offest / SSD1306_PAGE_POS_WIDTH < SCREEN_PAGE)
            {
                //设置坐标
                if (setDisplayBrush(x_offest, writtenPageCount + 1 + y_offest / SSD1306_PAGE_POS_WIDTH) != SSD1306_ERROR_NONE)
                    return SSD1306_ERROR_BRUSH_SET_FAILED;
                if (IIC_Wire->write(0x40) != IIC_ERROR_NONE) //表明写入data
                    return SSD1306_ERROR_WRITE_FAILED;
                //打印每行最后一行数据
                for (int count = 0; count < print_Width; count++)
                {
                    if (IIC_Wire->write((IMG_Arr[target_begin + count]) >> (SSD1306_PAGE_POS_WIDTH - pos_offest)) != IIC_ERROR_NONE)
                        return SSD1306_ERROR_WRITE_FAILED;
                    //缓冲区缓存
                    if (isFullBuffer)
                        buffer[buffer_target_begin + count] = (IMG_Arr[target_begin + count]) >> (SSD1306_PAGE_POS_WIDTH - pos_offest);
                }
                //结束一行的打印
                IIC_Wire->end();
            }
        }
        //更新指针坐标
        if (isFullBuffer)
        {
            cursor_Y = y_offest + sizey;
            //更新指针到字符整行
            cursor_Y += cursor_Y % ASCII_DATA_POS_WIDTH;
            cursor_X = 0;
        }
        return SSD1306_ERROR_NONE;
    }
}
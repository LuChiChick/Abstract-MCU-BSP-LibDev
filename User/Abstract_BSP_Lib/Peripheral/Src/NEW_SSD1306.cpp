#include "NEW_SSD1306.hpp"
// C标准定义
#include "stddef.h"

//宏定义
#define SSD1306_CMD 0x00         // SSD1306控制帧标识
#define SSD1306_DATA 0x40        // SSD1306数据帧标识
#define SSD1306_ADDR 0X3C        // SSD1306地址
#define IIC_SPEED_THRESHOLD 5    //信号稳定时延&刷新时延
#define ASCII_DATA_LENGTH 6      //默认字符集数据长度
#define ASCII_DATA_POS_WIDTH 8   //默认字符集数据像素宽度
#define SSD1306_PAGE_POS_WIDTH 8 //页像素宽度

namespace dev
{
    //使用ASCII_0806标准字符集
    using Monochrome_Screen_Resources::ASCII_0806;

    //默认构造函数
    SSD1306::SSD1306()
    {
        isInit_already = false;
    }

    /**
     * 带参构造函数
     * @param Abs_IIC_Master实例
     * @param ScreenX 屏幕X分辨率
     * @param ScreenY 屏幕Y分辨率
     */
    SSD1306::SSD1306(Abs_IIC_Master &IIC_Wire, uint8_t ScreenX, uint8_t ScreenY)
    {
        construct(IIC_Wire, ScreenX, ScreenY);
    }

    /**
     * 构造&初始化
     * @param Abs_IIC_Master实例
     * @param ScreenX 屏幕X分辨率
     * @param ScreenY 屏幕Y分辨率
     * @return SSD1306实例
     */
    SSD1306 &SSD1306::construct(Abs_IIC_Master &IIC_Wire, uint8_t ScreenX, uint8_t ScreenY)
    {
        //相关参数初始化
        this->IIC_Wire = &IIC_Wire;                                     //记录IIC对象
        this->SCREEN_X = ScreenX;                                       //屏幕长
        this->SCREEN_Y = ScreenY;                                       //屏幕宽
        this->SCREEN_PAGE_COUNT = ScreenY / SSD1306_PAGE_POS_WIDTH;     //屏幕页数计算
        this->SCREEN_PAGE_ASCII_CHAR_MAX = ScreenX / ASCII_DATA_LENGTH; //屏幕最大容量计算
        this->cursor_X = 0;                                             //屏幕X指针归位
        this->cursor_Y = 0;                                             //屏幕Y指针归位
        this->buffer = NULL;                                            //缓冲区初始化
        this->isFullBuffer = false;                                     //非全页缓存

        // IIC相关设置 指向目标地址 设置速度阈值
        this->IIC_Wire->focus_on(SSD1306_ADDR);
        this->IIC_Wire->set_SpeedThreshold(IIC_SPEED_THRESHOLD);

        //初始化完成
        isInit_already = true;

        //初始化屏幕参数&唤醒，执行200次直到成功，否则宣布初始化失败
        for (int count = 0; count < 200; count++)
            if (init_Screen() == MONOCHROME_SCREEN_ERROR_NONE)
                return *this;

        //初始化失败
        isInit_already = false;

        return *this;
    }

    /**
     *   初始化屏幕
     *   @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error SSD1306::init_Screen()
    {
        //初始化全程所需命令行
        //设置为字节垂直数据水平模式，屏幕分页，Y轴8像素点为一页
        const uint8_t initCmdLine[29] = {
            SSD1306_CMD, //表明输入cmd
            0xAE,        //--turn off oled panel
            0x00,        //---set low column address
            0x10,        //---set high column address
            0x40,        //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
            0x81,        //--set contrast control register
            0xCF,        // Set SEG Output Current Brightness
            0xA1,        //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
            0xC8,        // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
            0xA6,        //--set normal display
            0xA8,        //--set multiplex ratio(1 to 64)
            0x3f,        //--1/64 duty
            0xD3,        //-set display offset Shift Mapping RAM Counter (0x00~0x3F)
            0x00,        //-not offset
            0xd5,        //--set display clock divide ratio/oscillator frequency
            0x80,        //--set divide ratio, Set Clock as 100 Frames/Sec
            0xD9,        //--set pre-charge period
            0xF1,        // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
            0xDA,        //--set com pins hardware configuration
            0x12,        //
            0xDB,        //--set vcomh
            0x40,        // Set VCOM Deselect Level
            0x20,        //-Set Page Addressing Mode (0x00/0x01/0x02)
            0x02,        //
            0x8D,        //--set Charge Pump enable/disable
            0x14,        //--set(0x10) disable
            0xA4,        // Disable Entire Display On (0xa4/0xa5)
            0xA6,        // Disable Inverse Display On (0xa6/a7)
            0xAF,        // display ON
        };
        //写入上述初始化指令
        for (int count = 0; count < 29; count++)
        {
            //循环写入
            if (IIC_Wire->write(initCmdLine[count]) != IIC_ERROR_NONE)
                return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED; //连接失败
        }
        //结束通信
        IIC_Wire->end();
        //清理屏幕
        if (clear() != MONOCHROME_SCREEN_ERROR_NONE)
            return MONOCHROME_SCREEN_ERROR_CLEAR_SCREEN_FAILED; //清理屏幕失败
        //发送初始化信息
        if (printf(0, 0, "Screen Init OK!!\n\n"
                         "Powered by:\n\n"
                         "Abstract BSP Lib.\n"
                         "From LuChiChick.\n\n"
                         ">2022\\8\\6 Ver.") != MONOCHROME_SCREEN_ERROR_NONE)
            return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED; //输出流失败
        //无错误返回
        return MONOCHROME_SCREEN_ERROR_NONE;
    }

    /**
     *   指定坐标打印字符
     *   @param x_offest x坐标
     *   @param y_offest y坐标
     *   @param chr 输出字符
     *   @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error SSD1306::putchar(uint8_t x_offest, uint8_t y_offest, const char chr)
    {
        //正确初始化检查
        if (isInit_already == false)
            return MONOCHROME_SCREEN_ERROR_UNINITED; //未初始化
        //排查是否越界
        if (y_offest >= SCREEN_Y)
            return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y; // Y越界
        if (x_offest >= SCREEN_X)
            return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_X; // X越界

        //缓存字符到缓冲区
        draw_Char_ToBuffer(x_offest, y_offest, chr);

        //计算是否需要偏移像素点分两行打印(即坐标是否能被PagePos整除)
        uint8_t pos_offest = y_offest % SSD1306_PAGE_POS_WIDTH;

        //第一阶段打印，若字符打印坐标在整页上则一阶段结束打印即可，否则偏移像素打印并进入下一阶段打印
        {
            //设置打印笔刷位置
            if (set_Brush_Position(x_offest, y_offest / SSD1306_PAGE_POS_WIDTH) != MONOCHROME_SCREEN_ERROR_NONE)
                return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED; //连接失败

            //起始通信，表明连续写入data
            if (IIC_Wire->write(SSD1306_DATA) != IIC_ERROR_NONE)
                return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED; //连接失败

            //循环打印字符点阵数据，ASCII_0806字符为6个8位数据组成
            for (uint8_t count = 0; count < ((x_offest + ASCII_DATA_LENGTH) <= SCREEN_X ? ASCII_DATA_LENGTH : SCREEN_X - x_offest); count++)
            {
                //偏移pos_offest单位打印
                if (IIC_Wire->write((ASCII_0806[chr - ' '][count]) << pos_offest) != IIC_ERROR_NONE)
                    return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED; //连接失败
            }

            //通信结束
            IIC_Wire->end();
        }

        //判断是否需要打印下半行
        if (pos_offest > 0)
        {
            //判断下半行数据是否超过屏幕Y，如果越界则不用打印
            if ((y_offest / SSD1306_PAGE_POS_WIDTH) + 1 == SCREEN_PAGE_COUNT)
                return MONOCHROME_SCREEN_ERROR_NONE;

            //设置打印笔刷位置
            if (set_Brush_Position(x_offest, (y_offest / SSD1306_PAGE_POS_WIDTH) + 1) != MONOCHROME_SCREEN_ERROR_NONE)
                return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED;

            //起始通信，表明连续写入data
            if (IIC_Wire->write(SSD1306_DATA) != IIC_ERROR_NONE)
                return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED;

            //循环打印字符点阵数据，ASCII_0806字符为6个8位数据组成
            for (uint16_t count = 0; count < ((x_offest + ASCII_DATA_LENGTH) <= SCREEN_X ? ASCII_DATA_LENGTH : SCREEN_X - x_offest); count++)
            {
                //反向偏移数据,打印字符下半段
                if (IIC_Wire->write((ASCII_0806[chr - ' '][count]) >> (SSD1306_PAGE_POS_WIDTH - pos_offest)) != IIC_ERROR_NONE)
                    return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED;
            }

            //通信结束
            IIC_Wire->end();
        }

        return MONOCHROME_SCREEN_ERROR_NONE;
    }

}
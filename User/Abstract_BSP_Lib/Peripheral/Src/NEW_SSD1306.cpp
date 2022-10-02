#include "NEW_SSD1306.hpp"
// C标准定义
#include "stddef.h"

//宏定义
#define SSD1306_CMD 0x00          // SSD1306控制帧标识
#define SSD1306_DATA 0x40         // SSD1306数据帧标识
#define SSD1306_ADDR 0X3C         // SSD1306地址
#define IIC_DEF_SPEED_THRESHOLD 5 //信号稳定时延&刷新时延
#define SSD1306_PAGE_POS_WIDTH 8  //页像素宽度

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
    SSD1306::SSD1306(Abs_IIC_Master &IIC_Wire, uint8_t ScreenX, uint8_t ScreenY) : Abs_Monochrome_Screen()
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
        this->IIC_Wire = &IIC_Wire; //记录IIC对象
        this->SCREEN_X = ScreenX;   //屏幕长
        this->SCREEN_Y = ScreenY;   //屏幕宽

        // IIC相关设置 指向目标地址 设置速度阈值
        this->IIC_Wire->focus_on(SSD1306_ADDR);
        this->IIC_Wire->set_SpeedThreshold(IIC_DEF_SPEED_THRESHOLD);

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
            0xAE,        //--turn off oled panel 开启OLED面板
            0x00,        //---set low column address 设置列低位地址
            0x10,        //---set high column address 设置列高位地址
            0x40,        //--set start line address  Set Mapping RAM Display Start Line 设置起始行地址，0x40为第64位
            0x81,        //--set contrast control register 对比度寄存器
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
        // //清理屏幕
        // if (clear() != MONOCHROME_SCREEN_ERROR_NONE)
        //     return MONOCHROME_SCREEN_ERROR_CLEAR_SCREEN_FAILED; //清理屏幕失败

        //发送初始化信息
        if (printf("Screen Init OK!!\n\n"
                   "Powered by:\n\n"
                   "Abstract BSP Lib.\n"
                   "From LuChiChick.\n\n"
                   ">2022\\8\\6 Ver.") != IO_STREAM_ERROR_NONE)
            return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED; //输出流失败

        //无错误返回
        return MONOCHROME_SCREEN_ERROR_NONE;
    }

    /**
     * 指定位置输出图片
     * @param x_offest X坐标
     * @param y_offest y坐标
     * @param length 图片像素长度
     * @param width 图片像素宽度
     * @param IMG_Arr 图片数组
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error SSD1306::draw_IMG_at(uint16_t x_offest, uint16_t y_offest, uint16_t length, uint16_t width, const uint8_t *IMG_Arr)
    {
        //缓存图像
        save_IMG_To_Buffer(x_offest, y_offest, length, width, IMG_Arr);

        // //像素点偏移，计算打印位置偏移8位多少
        // uint8_t pos_offest = y_offest % 8;
        // //遍历打印
        // for (int uint8_print_Line = width / 8; uint8_print_Line > 0; uint8_print_Line--)
        // {
        //     for (int pos_Data_length = length; pos_Data_length > 0; pos_Data_length--)
        //     {
        //     }
        // }

        return MONOCHROME_SCREEN_ERROR_NONE;
    }

    /**
     * 设置打印起始笔刷位置
     * @param x_offest x坐标
     * @param page_offest 页面坐标
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error SSD1306::set_Brush_Position(uint16_t x_offest, uint16_t page_offest)
    {
        //越界审查
        if (x_offest >= SCREEN_X)
            return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_X;
        if (page_offest >= SCREEN_Y / 8)
            return MONOCHROME_SCREEN_ERROR_OUT_OF_RANGE_Y;
        //设置像素点位置命令
        if (IIC_Wire->write(0x00) == IIC_ERROR_NONE)
            if (IIC_Wire->write(0xb0 + page_offest) == IIC_ERROR_NONE)
                if (IIC_Wire->write(((x_offest & 0xf0) >> 4) | 0x10) == IIC_ERROR_NONE)
                    if (IIC_Wire->write((x_offest & 0x0f)) == IIC_ERROR_NONE)
                    {
                        IIC_Wire->end();
                        return MONOCHROME_SCREEN_ERROR_NONE;
                    }
        return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED;
    }

    /**
     * 清理屏幕
     * @param 无
     * @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error SSD1306::clear()
    {
        return MONOCHROME_SCREEN_ERROR_FUNCTION_UNREALIZED;
    }

}
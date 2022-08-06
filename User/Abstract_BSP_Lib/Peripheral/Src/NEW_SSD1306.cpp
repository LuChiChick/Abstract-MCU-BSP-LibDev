#include "NEW_SSD1306.hpp"

namespace dev
{
    /**
     *   初始化屏幕
     *   @return Monochrome_Screen_Error错误异常抛出
     */
    Monochrome_Screen_Error SSD1306::init_Screen()
    {
        //初始化全程所需命令行
        //设置为字节垂直数据水平模式，屏幕分页，Y轴8像素点为一页
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
                return MONOCHROME_SCREEN_ERROR_CONNECTION_FAILED; //连接失败
        }
        //结束通信
        IIC_Wire->end();
        //清理屏幕
        if (clear() != MONOCHROME_SCREEN_ERROR_NONE)
            return MONOCHROME_SCREEN_ERROR_CLEAR_SCREEN_FAILED; //清理屏幕失败
        //发送初始化信息
        if (printf(0, 0, "Screen Init OK!!\n\n      \
                        Powered by:\n\n             \
                        Abstract BSP Lib.\n         \
                        From LuChiChick.\n\n        \
                        >2022\\4\\17 Ver.") != MONOCHROME_SCREEN_ERROR_NONE)
            return MONOCHROME_SCREEN_ERROR_OUTSTREAM_FAILED; //输出流失败
        //无错误返回
        return MONOCHROME_SCREEN_ERROR_NONE;
    }
}
#ifndef __NEW_SSD1306__
#define __NEW_SSD1306__

#include "Abs_Monochrome_Screen.hpp"
#include "Abs_IIC.hpp"

//开发区名称空间
namespace dev
{
    //使用抽象单色屏类库及IIC
    using namespace cus;

    // SSD1306单色屏驱动类库
    class SSD1306 : public Abs_Monochrome_Screen
    {
    private:
        /**
         * 设置打印起始笔刷位置
         * @param x_offest x坐标
         * @param page_offest 页面坐标
         * @return Monochrome_Screen_Error错误异常抛出
         */
        Monochrome_Screen_Error set_Brush_Position(uint8_t x_offest, uint8_t page_offest);

        /**
         * 绘制字符到缓存
         * @param chr 缓存的字符
         */
        void draw_Char_ToBuffer(char chr);

        /**
         * 绘制字符到缓存
         * @param chr 缓存的字符
         * @param x_offest X坐标
         * @param y_offest Y坐标
         */
        void draw_Char_ToBuffer(uint8_t x_offest, uint8_t y_offest, char chr);

    protected:
        bool isInit_already; //是否初正确构造

        Abs_IIC_Master *IIC_Wire; // IIC通信类实例

        uint8_t SCREEN_PAGE_COUNT;          //屏幕竖向页，8像素点为一页
        uint8_t SCREEN_PAGE_ASCII_CHAR_MAX; //屏幕每页标准字符容量

        uint8_t cursor_X; // X指针，用于字符输出
        uint8_t cursor_Y; // Y指针，用于字符输出

        /**
         * 初始化屏幕
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error init_Screen();

    public:
        //默认构造函数
        SSD1306();

        /**
         * 带参构造函数
         * @param Abs_IIC_Master实例
         * @param ScreenX 屏幕X分辨率
         * @param ScreenY 屏幕Y分辨率
         */
        SSD1306(Abs_IIC_Master &IIC_Wire, uint8_t ScreenX, uint8_t ScreenY);

        /**
         * 构造&初始化
         * @param Abs_IIC_Master实例
         * @param ScreenX 屏幕X分辨率
         * @param ScreenY 屏幕Y分辨率
         * @return SSD1306实例
         */
        SSD1306 &construct(Abs_IIC_Master &IIC_Wire, uint8_t ScreenX, uint8_t ScreenY);

        /**
         * 指定坐标打印字符
         * @param x_offest x坐标
         * @param y_offest y坐标
         * @param chr 输出字符
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error putchar(uint8_t x_offest, uint8_t y_offest, const char chr);
    };
}

#endif

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
        Monochrome_Screen_Error set_Brush_Position(uint16_t x_offest, uint16_t page_offest);

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
        void draw_Char_ToBuffer_at(uint16_t x_offest, uint16_t y_offest, char chr);

    protected:

        Abs_IIC_Master *IIC_Wire; // IIC通信类实例

        uint16_t SCREEN_PAGE_COUNT; //屏幕竖向页，8像素点为一页

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
         * 清理屏幕
         * @param 无
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error clear();

        /**
         * 指定位置绘制像素点
         * @param x_offest X坐标
         * @param y_offest Y坐标
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_Pixel_at(uint16_t x_offest, uint16_t y_offest);

        /**
         * 指定坐标打印字符
         * @param x_offest x坐标
         * @param y_offest y坐标
         * @param chr 输出字符
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error putchar_at(uint16_t x_offest, uint16_t y_offest, const char chr);

        /**
         * 指定位置输出图片
         * @param x_offest X坐标
         * @param y_offest y坐标
         * @param length 图片像素长度
         * @param width 图片像素宽度
         * @param IMG_Arr 图片数组
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_IMG_at(uint16_t x_offest, uint16_t y_offest, uint16_t length, uint16_t width, const uint8_t *IMG_Arr);

        /**
         * 向默认位置(0,0)绘制图形
         * @param length 图片像素长度
         * @param width 图片像素宽度
         * @param IMG_Arr 图片数组,每列8位点阵,字节垂直数据水平
         * @return Monochrome_Screen_Error错误异常抛出
         */
        virtual Monochrome_Screen_Error draw_IMG(uint16_t length, uint16_t width, const uint8_t *IMG_Arr);
    };
}

#endif

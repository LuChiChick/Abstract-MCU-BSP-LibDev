#ifndef __CUS__SSD1306__
#define __CUS__SSD1306__

#include "Abs_BaseIO.hpp"

namespace cus
{
    // OLED相关错误反馈
    enum SSD1306_Error
    {
        SSD1306_ERROR_NONE,                  //操作成功
        SSD1306_ERROR_UNINITED,              //未正确初始化
        SSD1306_ERROR_TARGET_NOT_EXIST,      //目标设备不存在
        SSD1306_ERROR_WRITE_FAILED,          //命令写入失败
        SSD1306_ERROR_CLEAR_FAILED,          //清理屏幕失败
        SSD1306_ERROR_OUT_OF_RANGE_X,        // X出界
        SSD1306_ERROR_OUT_OF_RANGE_Y,        // Y出界
        SSD1306_ERROR_BRUSH_SET_FAILED,      //打印笔刷位置指针设置错误
        SSD1306_ERROR_PRINT_FAILED,          //打印数据错误
        SSD1306_ERROR_DRAW_INT_FAILED,       //绘制整数失败
        SSD1306_ERROR_DRAW_DEC_FAILED,       //绘制浮点数失败
        SSD1306_ERROR_DRAW_STR_FAILED,       //绘制字符串失败
        SSD1306_ERROR_SCREEN_ROLL_FAILED,    //滚动屏幕错误
        SSD1306_ERROR_BUFFER_SIZE_INCORRECT, //缓冲区大小不匹配
    };

    class SSD1306
    {
    private:
        bool isInit_already; //是否初正确构造

        Abs_IIC_Master *IIC_Wire; // IIC通信类实例

        uint8_t SCREEN_X;                   //屏幕横向像素点宽
        uint8_t SCREEN_Y;                   //屏幕竖向像素点宽度
        uint8_t SCREEN_PAGE;                //屏幕竖向页，8像素点为一页
        uint8_t SCREEN_PAGE_ASCII_CHAR_MAX; //屏幕每页标准字符容量

        uint8_t cursor_X; // X指针
        uint8_t cursor_Y; // Y指针

        uint8_t *buffer;   //缓冲区
        bool isFullBuffer; //是否全页缓存

        /**
         *   设置打印起始笔刷位置
         *   @param x_offest x坐标
         *   @param page_offest 页面坐标
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error setDisplayBrush(uint8_t x_offest, uint8_t page_offest);
        /**
         *   使用类内信息初始化屏幕
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error initScreen();
        /**
         * 缓冲区清理
         * @return 无
         */
        void clearBuffer();
        /**
         * 绘制字符到缓存
         * @param chr 缓存的字符
         */
        void drawCharToBuffer(char chr);
        /**
         * 绘制字符到缓存
         * @param chr 缓存的字符
         * @param x_offest X坐标
         * @param y_offest Y坐标
         */
        void drawCharToBuffer(uint8_t x_offest, uint8_t y_offest, char chr);
        /**
         *   递归打印整数
         *   @param num 输出整数
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error drawInteger(int num);
        /**
         *   指定坐标打印整数,仅用于内部，带有坐标更新
         *   @param cursor_X x指针
         *   @param cursor_Y y指针
         *   @param num 输出整数
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error drawInteger(uint8_t &cursor_X, uint8_t &cursor_Y, int num);
        /**
         *   打印浮点数
         *   @param num 输出浮点数
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error drawDecimal(double num);
        /**
         *   指定坐标打印浮点数,仅用于内部，带有坐标更新
         *   @param cursor_X x指针
         *   @param cursor_Y y指针
         *   @param num 输出浮点数
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error drawDecimal(uint8_t &cursor_X, uint8_t &cursor_Y, double num);
        /**
         *   指定坐标字符串,仅用于内部，带有坐标更新
         *   @param cursor_X x指针
         *   @param cursor_Y y指针
         *   @param string 输出uint8_t字符串
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error drawString(uint8_t &cursor_X, uint8_t &cursor_Y, const uint8_t *string);

    public:
        //构造函数
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
         *   颠倒颜色
         *   @param YesOrNo 是否颠倒颜色
         */
        SSD1306_Error colorTurn(bool YesOrNo);
        /**
         *   颠倒屏幕
         *   @param YesOrNo 是否颠倒屏幕
         */
        SSD1306_Error displayTurn(bool YesOrNo);
        /**
         *   清理屏幕
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error clear();
        /**
         *   设置屏幕全页缓存
         *   @param lpBuffer 缓存数据指针
         *   @param size 缓冲区长度
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error setFullBuffer(uint8_t *lpBuffer, uint16_t size);
        /**
         *   设置屏幕轻量级缓存,仅支持字符缓存
         *   @param lpBuffer 缓存数据指针
         *   @param size 缓冲区长度
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error setLightBuffer(uint8_t *lpBuffer, uint16_t size);
        /**
         *   向屏幕输出一个字符
         *   @param chr 需要打印的字符
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error putchar(const char chr);
        /**
         *   指定坐标打印字符
         *   @param x_offest x坐标
         *   @param y_offest y坐标
         *   @param chr 输出字符
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error putchar(uint8_t x_offest, uint8_t y_offest, const char chr);
        /**
         *   基于内部指针的控制台格式化输出
         *   @param lpFormatString 格式化输出字符串
         *   @param ... 要打印的内容参数，支持如下:
         *   @param -%d 输出整数
         *   @param -%f 输出浮点数
         *   @param -%s 输出字符串
         *   @param -%c 输出字符
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error printf(const char *lpFormatString, ...);
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
        SSD1306_Error printf(uint8_t x_offest, uint8_t y_offest, const char *lpFormatString, ...);
        /**
         *   全页滚动屏幕
         *   @param pos_count 滚动像素点计数
         *   @return SSD1306_Error异常抛出
         */
        SSD1306_Error screenRoll(uint8_t pos_count);
        /**
         *   指定位置输出图片
         *   @param x_offest X坐标
         *   @param y_offest y坐标
         *   @param sizex 图片长度
         *   @param sizey 图片宽度
         *   @param IMG_Arr 图片数组
         */
        SSD1306_Error drawIMG(uint8_t x_offest, uint8_t y_offest, uint8_t sizex, uint8_t sizey, const uint8_t *IMG_Arr);
    };

    //整活图像区，放心，基于编译器机制，在某个元素未被使用时是不会被编译的
    //也就是，图库尽管扩充，不会占用多余的宝贵flash空间

    // 8x8
    const uint8_t LaughingFace[8] = {0x04, 0x22, 0x44, 0x40, 0x40, 0x44, 0x22, 0x04};
    // 100x100
    const unsigned char EmmmCat[1300] = {
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0X7F, 0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X7F, 0X7F, 0X3F, 0XBF, 0X3F, 0X3F, 0XBF, //
        0X3F, 0XBF, 0X3F, 0X3F, 0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X7F, 0X3F, //
        0X9F, 0XCF, 0XAF, 0XE7, 0XB1, 0XFC, 0XBA, 0X6E, 0X3D, 0X16, 0X3E, 0XF6, 0XDC, 0X79, 0XE1, 0XA7, //
        0X5F, 0X9F, 0X3F, 0X7F, 0X7F, 0X7F, 0X7F, 0X7F, 0X7F, 0X3F, 0X3F, 0X9F, 0XDF, 0XAF, 0XE7, 0XD3, //
        0XF9, 0X5D, 0XF5, 0X3D, 0X6D, 0XF9, 0XB3, 0XEB, 0X47, 0X8F, 0X23, 0X01, 0X5C, 0X20, 0X4F, 0X30, //
        0X0E, 0X21, 0X0E, 0X11, 0X4E, 0X90, 0X6F, 0X90, 0X2A, 0X54, 0X21, 0X83, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X7F, 0X1F, 0XC7, //
        0XB1, 0XFC, 0XAE, 0XFF, 0X2A, 0X1F, 0X0D, 0X06, 0X03, 0X01, 0X80, 0X00, 0X00, 0X10, 0X00, 0X80, //
        0X01, 0X43, 0X0F, 0X05, 0X0F, 0X17, 0X1D, 0X1F, 0X0A, 0X1F, 0X1E, 0X0B, 0X1E, 0X17, 0X1D, 0X1E, //
        0X0B, 0X0F, 0X82, 0X01, 0X01, 0X88, 0X00, 0X40, 0X00, 0X21, 0X07, 0X1F, 0XFD, 0X77, 0XDC, 0XF1, //
        0X42, 0X96, 0X3F, 0XFF, 0XFE, 0XE1, 0X00, 0X15, 0XAA, 0X51, 0XAE, 0X11, 0X00, 0XC4, 0XFE, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0X47, 0X90, 0X7E, 0XEB, 0XBE, 0XFF, 0X03, 0X00, 0X08, 0X80, 0X00, 0X02, 0X20, 0X00, 0X10, 0X00, //
        0X04, 0X00, 0X80, 0X00, 0X08, 0XC0, 0XA0, 0XE0, 0XBA, 0XFC, 0XAC, 0X7C, 0X35, 0X1C, 0X38, 0X00, //
        0X01, 0XC0, 0XF0, 0X60, 0XE1, 0X00, 0X10, 0X00, 0X00, 0X00, 0X10, 0X00, 0X00, 0X08, 0X00, 0X00, //
        0X40, 0X03, 0X0F, 0X75, 0XFF, 0XEE, 0XBA, 0XE0, 0X07, 0X3F, 0XFF, 0XFE, 0XFE, 0X3E, 0X1E, 0XDE, //
        0X1F, 0X9F, 0X1F, 0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XC0, 0X1A, 0X57, 0XFD, 0XAF, 0XFF, 0XD0, 0X00, 0X00, 0X08, 0X00, 0X80, //
        0X04, 0X00, 0X40, 0X02, 0X20, 0X00, 0X10, 0X00, 0X00, 0X0A, 0X0F, 0X1D, 0X37, 0X0D, 0X01, 0XC0, //
        0XE0, 0X40, 0XC4, 0X20, 0X00, 0X00, 0X11, 0X01, 0X00, 0XC0, 0XE0, 0XB4, 0XE0, 0XF1, 0XA0, 0XF0, //
        0X60, 0XC1, 0XC0, 0X80, 0X08, 0X00, 0X00, 0XC9, 0XFF, 0XB6, 0XFF, 0X6D, 0X92, 0X00, 0XFF, 0XFF, //
        0XFE, 0XF8, 0XF1, 0XF2, 0XF1, 0XF2, 0XF1, 0XFC, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC, 0XF8, 0XC5, 0X17, 0X5E, 0XFB, 0XAF, //
        0XFE, 0XF0, 0X40, 0X00, 0X08, 0X00, 0X00, 0X88, 0X00, 0X04, 0X00, 0X80, 0X02, 0X40, 0X01, 0X20, //
        0X00, 0X10, 0X00, 0X00, 0X03, 0X41, 0X09, 0X00, 0X00, 0X42, 0X00, 0X50, 0XBD, 0XFF, 0XD6, 0X7F, //
        0X01, 0X04, 0X00, 0X01, 0X83, 0X07, 0X1A, 0XFF, 0XED, 0XBE, 0XF5, 0X5F, 0X96, 0X4F, 0XE1, 0XF2, //
        0XFC, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X7F, 0X1F, 0XC7, 0X70, //
        0XFE, 0XD5, 0X7F, 0X3B, 0X0E, 0X01, 0X00, 0X40, 0X04, 0X20, 0X00, 0X00, 0X08, 0X00, 0X40, 0X00, //
        0X08, 0X00, 0X80, 0X00, 0X08, 0X00, 0X40, 0X02, 0X20, 0X00, 0X00, 0X04, 0X40, 0X00, 0X10, 0X00, //
        0X03, 0X05, 0X0F, 0X3F, 0X2D, 0X3C, 0X08, 0X00, 0X80, 0X00, 0X54, 0XEF, 0XBE, 0XF7, 0XFD, 0X03, //
        0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X01, //
        0X58, 0XEF, 0XBD, 0XF7, 0X6E, 0XC3, 0XC1, 0X40, 0XE0, 0X60, 0XE8, 0XE0, 0XA4, 0X00, 0X00, 0X02, //
        0X00, 0X20, 0X00, 0X04, 0X80, 0X00, 0X10, 0X00, 0X04, 0X00, 0X40, 0X00, 0X04, 0X00, 0X40, 0X04, //
        0X00, 0X00, 0X40, 0X08, 0X01, 0X00, 0X00, 0X44, 0X00, 0X02, 0X00, 0X00, 0X20, 0X00, 0X00, 0XFF, //
        0XBE, 0XEB, 0X7F, 0XD2, 0X00, 0X3F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFE, 0XFC, 0XF1, 0XF2, 0XF7, 0X4B, 0X07, 0XFD, 0XD7, 0X7F, 0XFD, 0X83, 0X08, //
        0X00, 0X02, 0X80, 0X01, 0X20, 0X00, 0X00, 0X04, 0X00, 0X20, 0X00, 0X10, 0X02, 0X00, 0X00, 0X10, //
        0X00, 0X82, 0X00, 0X20, 0X00, 0X04, 0X00, 0X00, 0X22, 0X00, 0X00, 0X08, 0X00, 0X82, 0X00, 0X01, //
        0X20, 0X00, 0X00, 0X6D, 0XFF, 0XAD, 0XFF, 0XAA, 0X04, 0XEF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X02, 0X00, 0XFF, 0XDD, //
        0X77, 0XFF, 0X48, 0X00, 0X00, 0X08, 0X00, 0X00, 0X88, 0X00, 0X00, 0X21, 0X00, 0X08, 0X00, 0X00, //
        0X42, 0X00, 0X21, 0X00, 0X00, 0X10, 0X00, 0X08, 0X00, 0X00, 0X81, 0X10, 0X00, 0X04, 0X00, 0X81, //
        0X00, 0X20, 0X00, 0X08, 0X00, 0X04, 0X00, 0XBB, 0XFF, 0X6B, 0XFE, 0X55, 0X80, 0XBF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XE0, 0X0A, 0XFF, 0XAB, 0XFF, 0XDD, 0X00, 0X00, 0X80, 0X11, 0X00, 0XC0, 0XE0, 0XB0, 0XE8, //
        0X70, 0XF2, 0X58, 0XF8, 0XA8, 0X7C, 0X58, 0X74, 0XB8, 0X78, 0XAA, 0X78, 0XB9, 0X68, 0X78, 0X58, //
        0XF8, 0X34, 0X58, 0XF0, 0XB8, 0XF0, 0XE0, 0X80, 0X22, 0X00, 0X00, 0X22, 0XFF, 0XF7, 0X5D, 0XF7, //
        0X08, 0XE2, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0, 0XCA, 0X0D, 0X3F, 0X6B, 0X3E, 0X58, 0X74, 0X3C, 0X57, //
        0X3F, 0X85, 0XCE, 0XF1, 0XF9, 0XFC, 0XFE, 0XFE, 0XFE, 0XFE, 0XFE, 0XFE, 0XFE, 0XFE, 0XFE, 0XFE, //
        0XFE, 0XFE, 0XFE, 0XFE, 0XFE, 0XFE, 0XF8, 0XC2, 0X8F, 0X3E, 0X2B, 0X7E, 0X2C, 0X78, 0X3C, 0X6A, //
        0X1F, 0XBD, 0X87, 0XCD, 0XE2, 0XFC, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, //
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, //
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, //
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, //
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, //
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, //
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, //
        0X0F, 0X0F, 0X0F, 0X0F};                                                                        //
    // 80*60
    const uint8_t LieDownCat[640] = {
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X80, 0X80, 0XC0, 0XC0, //
        0XE0, 0XE0, 0XF0, 0XF0, 0XF8, 0XF8, 0XF8, 0XFC, 0XFC, 0XFC, 0X7C, 0X7E, 0X7E, 0X7E, 0X7E, 0X7E, //
        0X7E, 0X7E, 0X7E, 0X7E, 0X7E, 0X7E, 0X7E, 0X7E, 0XFC, 0XFC, 0XFC, 0XF8, 0XF8, 0XF0, 0XF0, 0XE0, //
        0XE0, 0XC0, 0XC0, 0X80, 0X00, 0X80, 0X80, 0XC0, 0XC0, 0XE0, 0XC0, 0XC0, 0X80, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0XC0, 0XE0, 0XF0, 0XF8, 0XFC, 0XFE, 0XFE, 0X7F, 0X3F, 0X1F, 0X1F, //
        0X0F, 0X07, 0X07, 0X03, 0X01, 0X01, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0XC0, 0XF0, 0XF8, 0XFC, //
        0XFC, 0XFE, 0X7E, 0XFE, 0XFC, 0XFC, 0XF8, 0XF0, 0XF0, 0XE0, 0XC0, 0XC1, 0XC3, 0XC3, 0XC7, 0XC7, //
        0XEF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X7F, 0X0F, 0X0F, 0X1F, 0X3F, 0X7F, 0XFE, 0XFC, 0XFC, //
        0XF8, 0XF0, 0XF0, 0XE0, 0XC0, 0X80, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0XE0, 0XF8, 0XFE, 0XFF, 0XFF, 0XFF, 0X3F, 0X0F, 0X03, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X80, 0XE0, 0XF8, 0XF8, 0XFC, 0XFE, 0XFF, 0X7F, 0X3F, 0X1F, //
        0X07, 0X01, 0X00, 0X00, 0X01, 0X01, 0X03, 0X07, 0X07, 0X07, 0X07, 0X07, 0X07, 0X07, 0X07, 0X07, //
        0X07, 0X07, 0X07, 0X07, 0X03, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X01, //
        0X03, 0X07, 0X0F, 0X1F, 0X7F, 0XFF, 0XFF, 0XFE, 0XF8, 0XE0, 0X80, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X80, 0XF0, 0XFC, 0XFE, 0XFF, 0XFF, 0X3F, 0X0F, 0X07, 0X01, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0XC0, 0XE0, 0XF0, 0XF0, 0XE0, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0XE0, 0XE0, 0XE0, 0XE0, 0XC0, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X1F, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC, 0XE0, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X03, 0X7F, 0XFF, 0XFF, 0XFF, 0XFF, 0XF0, 0XC0, 0X80, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X61, 0XFB, 0XFB, 0XF9, 0XF8, 0XF0, 0XE0, //
        0XF0, 0XE0, 0XE0, 0XE0, 0XE0, 0XE0, 0XE3, 0XC7, 0X07, 0X03, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0XE0, 0XF0, 0XFF, 0XFF, 0XFF, 0XFF, 0X7F, 0X01, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X03, 0X0F, 0X1F, 0X3F, 0X7F, 0XFF, 0XFE, 0XFC, 0XF8, 0XF0, 0XF0, 0XE0, 0XE0, //
        0XC0, 0XC0, 0X80, 0X80, 0X01, 0X03, 0X07, 0X0F, 0X1F, 0X1F, 0X1F, 0X1F, 0X0E, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X03, 0X03, 0X07, 0X07, 0X07, //
        0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X0F, 0X07, 0X03, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X80, 0X80, //
        0XC0, 0XE0, 0XF0, 0XFC, 0XFF, 0XFF, 0XFF, 0X3F, 0X0F, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, 0X01, 0X03, 0X03, 0X07, 0X07, 0X0F, //
        0X0F, 0X0F, 0X1F, 0X1F, 0X3F, 0X3F, 0X3F, 0X7E, 0X7E, 0X7E, 0X7C, 0XFC, 0XFC, 0XFC, 0XF8, 0XF8, //
        0XF8, 0XF8, 0XF8, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, 0XF0, //
        0XF0, 0XF0, 0XF0, 0XF8, 0XF8, 0XF8, 0XF8, 0XFC, 0XFC, 0XFC, 0X7E, 0X7E, 0X7E, 0X3F, 0X3F, 0X1F, //
        0X1F, 0X0F, 0X07, 0X07, 0X03, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X01, //
        0X01, 0X01, 0X01, 0X01, 0X03, 0X03, 0X03, 0X03, 0X03, 0X03, 0X03, 0X03, 0X03, 0X03, 0X03, 0X03, //
        0X01, 0X01, 0X01, 0X01, 0X01, 0X01, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
        0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, //
    };
}
#endif
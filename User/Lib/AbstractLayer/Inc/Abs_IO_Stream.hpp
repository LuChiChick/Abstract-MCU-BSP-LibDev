#ifndef __CUS__ABS_IO_STREAM__
#define __CUS__ABS_IO_STREAM__

namespace cus
{
    //输入输出流错误枚举类型
    enum IO_Stream_Error
    {
        IO_STREAM_ERROR_NONE,           //无错误
        IO_STREAM_ERROR_PUTCHAR_FAILED, //输出字节失败
        IO_STREAM_ERROR_PRINTF_FAILED,  //格式化输入输出失败
    };

    //抽象输出流，提供printf及<<重定向运算
    class Abs_OStream
    {
        //私有不可覆写基础功能实现，主要是优化printf处理流程
    private:
        /**
         * 递归打印整数
         * @param num 要打印的整数
         */
        IO_Stream_Error printInteger(int num);
        /**
         * 打印浮点数
         * @param num 要打印的浮点数
         */
        IO_Stream_Error printDecimal(double num);

    protected:
        //小数输出位数设置
        int DECIMAL_HOLD_MAX;

    public:
        //默认构造函数
        Abs_OStream();

        /**
         *   输出一个字节数据
         *   @param chr 需要输出的字节数据
         *   @return IO_Stream_Error异常抛出
         */
        virtual IO_Stream_Error putchar(const char chr) = 0;

        /**
         *   格式化输出
         *   @param lpFormatString 格式化输出字符串
         *   @param ... 要输出的内容参数，支持如下:
         *   @param -%d 输出整数
         *   @param -%f 输出浮点数
         *   @param -%s 输出字符串
         *   @param -%c 输出字符
         *   @return IO_Stream_Error异常抛出
         */
        virtual IO_Stream_Error printf(const char *lpFormatString, ...);

        /**
         * <<运算符重定向
         * @param str 输出的字符串
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_OStream &operator<<(const char *str);

        /**
         * <<运算符重定向
         * @param chr 输出的字符串
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_OStream &operator<<(const char chr);

        /**
         * <<运算符重定向
         * @param num 输出的数字
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_OStream &operator<<(const int num);

        /**
         * <<运算符重定向
         * @param num 输出的数字
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_OStream &operator<<(const double num);

        /**
         * <<运算符重定向
         * @param num 输出的数字
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_OStream &operator<<(const float num);

        /**
         *   设置格式化输入输出最大小数位数
         *   @param count 需要设置的小数保留位数
         *   @return 无
         */
        virtual void Set_Decimal_Hold(int BaudRate);
    };

    //抽象输入流，提供scanf及>>重定向运算
    class Abs_IStream
    {
        /**
         * 获取一个字节数据
         * @return 获取的字节数据
         */
        virtual char getchar() = 0;

        /**
         *   格式化输入
         *   @param lpFormatString 格式化输入字符串
         *   @param ... 要读取的内容参数，支持如下:
         *   @param -%d 读取整数
         *   @param -%f 读取单精度浮点数
         *   @param -%s 读取字符串
         *   @param -%c 读取字符
         *   @param -%xx 标准输入输出支持的其他标签
         *   @return IO_Stream_Error异常抛出
         */
        virtual IO_Stream_Error scanf(const char *lpFormatString, ...) = 0;

        /**
         * >>运算符重定向
         * @param str 读取字符串
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_IStream &operator>>(char *str);

        /**
         * >>运算符重定向
         * @param chr 读取字符
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_IStream &operator>>(char &chr);

        /**
         * >>运算符重定向
         * @param num 读取数字
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_IStream &operator>>(int &num);

        /**
         * >>运算符重定向
         * @param num 读取数字
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_IStream &operator>>(double &num);

        /**
         * >>运算符重定向
         * @param num 读取数字
         * @return Abs_OStream& 实例自身
         */
        virtual Abs_IStream &operator>>(float &num);
    };

}

#endif

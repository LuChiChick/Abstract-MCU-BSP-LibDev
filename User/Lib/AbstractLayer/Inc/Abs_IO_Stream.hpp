#ifndef __CUS__ABS_IO_STREAM__
#define __CUS__ABS_IO_STREAM__

namespace cus
{
    //抽象输出流，提供printf及<<重定向运算，可重定义返回类型
    template <class Error_Return>
    class Abs_OStream
    {

    public:
        /**
         *   向屏幕输出一个字符
         *   @param chr 需要打印的字符
         *   @return 特定异常抛出
         */
        virtual Error_Return putchar(const char chr) = 0;

        /**
         *   基于内部指针的控制台格式化输出
         *   @param lpFormatString 格式化输出字符串
         *   @param ... 要打印的内容参数，支持如下:
         *   @param -%d 输出整数
         *   @param -%f 输出浮点数
         *   @param -%s 输出字符串
         *   @param -%c 输出字符
         *   @return 特定异常抛出
         */
        virtual Error_Return printf(const char *lpFormatString, ...) = 0;
    };
}

#endif

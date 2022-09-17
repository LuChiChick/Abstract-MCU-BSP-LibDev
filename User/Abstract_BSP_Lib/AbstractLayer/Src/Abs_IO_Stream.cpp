#include "Abs_IO_Stream.hpp"
#include "stdarg.h"
namespace cus
{
    //默认构造函数
    Abs_OStream::Abs_OStream()
    {
        //默认为5位小数保留
        DECIMAL_HOLD_MAX = 5;
    }
    /**
     *   格式化输出默认实现
     *   @param lpFormatString 格式化输出字符串
     *   @param ... 要输出的内容参数，支持如下:
     *   @param -%d 输出整数
     *   @param -%f 输出浮点数
     *   @param -%s 输出字符串
     *   @param -%c 输出字符
     *   @return IO_Stream_Error异常抛出
     */
    IO_Stream_Error Abs_OStream::printf(const char *lpFormatString, ...)
    {
        //初始化参数列表，指向不定参第一个参数
        va_list args;
        va_start(args, lpFormatString);

        //轮询数据
        while (*lpFormatString != '\0')
        {
            if (*lpFormatString == '%')
            {
                //筛查格式化标签
                switch (*(lpFormatString + 1))
                {
                //数型
                case 'd':
                    printInteger(va_arg(args, int));
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    continue;
                case 'f':
                    printDecimal(va_arg(args, double));
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    continue;
                case 's':
                    printf(va_arg(args, const char *));
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    continue;
                case 'c':
                    putchar(va_arg(args, int));
                    //跃迁格式化字符串指针
                    lpFormatString += 2;
                    continue;
                //不在检测范围内的标签
                default:
                    break;
                }
            }
            //打印常规字符
            putchar(*(lpFormatString++)); //打印并更新指针
        }

        //无错误返回
        return IO_STREAM_ERROR_NONE;
    }

    /**
     * <<运算符重定向
     * @param chr 输出的字符串
     * @return Abs_OStream& 实例自身
     */
    Abs_OStream &Abs_OStream::operator<<(const char chr)
    {
        putchar(chr);
        return *this;
    }

    /**
     * <<运算符重定向
     * @param str 输出的字符串
     * @return Abs_OStream& 实例自身
     */
    Abs_OStream &Abs_OStream::operator<<(const char *str)
    {
        printf(str);
        return *this;
    }

    /**
     * <<运算符重定向
     * @param num 输出的数字
     * @return Abs_OStream& 实例自身
     */
    Abs_OStream &Abs_OStream::operator<<(const int num)
    {
        printf("%d", num);
        return *this;
    }

    /**
     * <<运算符重定向
     * @param num 输出的数字
     * @return Abs_OStream& 实例自身
     */
    Abs_OStream &Abs_OStream::operator<<(const double num)
    {
        printf("%f", num);
        return *this;
    }

    /**
     * <<运算符重定向
     * @param num 输出的数字
     * @return Abs_OStream& 实例自身
     */
    Abs_OStream &Abs_OStream::operator<<(const float num)
    {
        printf("%f", num);
        return *this;
    }

    /**
     * 递归打印整数
     * @param num 要打印的整数
     */
    IO_Stream_Error Abs_OStream::printInteger(int num)
    {
        //处理正负号
        if (num < 0)
        {
            //打印负号并检查是否错误
            if (putchar('-') != IO_STREAM_ERROR_NONE)
                return IO_STREAM_ERROR_PUTCHAR_FAILED;

            return printInteger(-num);
        }

        //处理常规值
        if (num < 10) //递归出口
            return putchar('0' + num);
        else
        {
            //整除10为打印前一位，取余为打印自身
            if (printInteger(num / 10) != IO_STREAM_ERROR_NONE)
                return IO_STREAM_ERROR_PRINTF_FAILED;

            return printInteger(num % 10);
        }
    }

    /**
     * 打印浮点数
     * @param num 要打印的浮点数
     */
    IO_Stream_Error Abs_OStream::printDecimal(double num)
    {
        //处理原来的整数部分
        if (printInteger((int)(num * 1.0)) != IO_STREAM_ERROR_NONE)
            return IO_STREAM_ERROR_PRINTF_FAILED;

        //去掉已经打印的整数部分
        num = num - (int)num;

        //打印小数点
        if (putchar('.') != IO_STREAM_ERROR_NONE)
            return IO_STREAM_ERROR_PUTCHAR_FAILED;

        //扩大倍数转将小数部分换成整数来解决问题
        for (int count = 0; count < DECIMAL_HOLD_MAX; count++)
            num *= 10;
        //忽略数部分前进保留位到整数后多余的0
        while ((int)num % 10 == 0 && (int)num != 0)
            num = (int)num / 10;
        //处理正负号，前面打印整数部分时已经带符号了
        if (num < 0)
            num = -num;
        return printInteger((int)num);
    }

    /**
     *   设置格式化输入输出最大小数位数
     *   @param count 需要设置的小数保留位数
     *   @return 无
     */
    void Abs_OStream::set_Decimal_Hold(int count)
    {
        DECIMAL_HOLD_MAX = count;
    }

    /**
     * >>运算符重定向
     * @param str 读取字符串
     * @return Abs_OStream& 实例自身
     */
    Abs_IStream &Abs_IStream::operator>>(char *str)
    {
        scanf("%s", str);
        return *this;
    }

    /**
     * >>运算符重定向
     * @param chr 读取字符
     * @return Abs_OStream& 实例自身
     */
    Abs_IStream &Abs_IStream::operator>>(char &chr)
    {
        scanf("%c", &chr);
        return *this;
    }

    /**
     * >>运算符重定向
     * @param num 读取数字
     * @return Abs_OStream& 实例自身
     */
    Abs_IStream &Abs_IStream::operator>>(int &num)
    {
        scanf("%d", &num);
        return *this;
    }

    /**
     * >>运算符重定向
     * @param num 读取数字
     * @return Abs_OStream& 实例自身
     */
    Abs_IStream &Abs_IStream::operator>>(double &num)
    {
        scanf("%lf", &num);
        return *this;
    }

    /**
     * >>运算符重定向
     * @param num 读取数字
     * @return Abs_OStream& 实例自身
     */
    Abs_IStream &Abs_IStream::operator>>(float &num)
    {
        scanf("%f", &num);
        return *this;
    }
}
#ifndef __CUS__ABS_GPIO_WIRE__
#define __CUS__ABS_GPIO_WIRE__

#include "stdint.h"

namespace cus
{

    //电平枚举类型
    enum GPIO_Wire_Level
    {
        GPIO_WIRE_LEVEL_UNKNOWN = -1, //未知电平
        GPIO_WIRE_LEVEL_LOW,          //低电平
        GPIO_WIRE_LEVEL_HIGH,         //高电平
    };

    //引脚枚举类型
    enum GPIO_Wire_Mode
    {
        GPIO_WIRE_MODE_UNKNOWN,         //未知模式
        GPIO_WIRE_MODE_OUTPUT_PP,       //推免输出模式
        GPIO_WIRE_MODE_OUTPUT_OD,       //开漏输出模式
        GPIO_WIRE_MODE_INPUT_PULL_UP,   //拉高输入模式
        GPIO_WIRE_MODE_INPUT_PULL_DOWN, //拉低输入模式
        GPIO_WIRE_MODE_INPUT_NO_PULL,   //无上下拉普通输入
    };

    // Wire类错误类型反馈
    enum GPIO_Wire_Error
    {
        GPIO_WIRE_ERROR_NONE,                //执行成功
        GPIO_WIRE_ERROR_UNINITED,            //未正确初始化
        GPIO_WIRE_ERROR_MODE_UNKNOWN,        //引脚初始化模式未知
        GPIO_WIRE_ERROR_MODE_INCORRECT,      //错误的引脚模式
        GPIO_WIRE_ERROR_LEVEL_UNKNOWN,       //引脚未知电平错误
        GPIO_WIRE_ERROR_FUNCTION_UNREALIZED, //功能未实现
    };

    //抽象GPIO封装类
    class Abs_GPIO_Wire
    {
    public:
        /**
         * 设置引脚输出模式
         * @param Mode -引脚模式,
         * 详见GPIO_Mode枚举类型
         * @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error setAs(GPIO_Wire_Mode Mode) = 0;
        /**
         * 输出电平
         * @param Level -决定输出的电平高低
         * 详见GPIO_Level枚举类型
         * @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error write(GPIO_Wire_Level Level) = 0;

        /**
         * 读取电平高低
         * @return GPIO_Wire_Level 电平高低
         */
        virtual GPIO_Wire_Level readLevel() = 0;

        /**
         * 获取当前模式
         * @return GPIO_Wire_Mode 引脚模式
         */
        virtual GPIO_Wire_Mode getMode() = 0;

        /**
         * 翻转电平
         * @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error toggle() = 0;
    };
}
#endif
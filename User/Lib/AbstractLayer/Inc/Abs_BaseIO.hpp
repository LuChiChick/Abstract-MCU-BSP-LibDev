#ifndef __CUS__ABS_BASE_IO__
#define __CUS__ABS_BASE_IO__

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
        GPIO_WIRE_ERROR_UNREALIZED_FUNCTION, //功能未实现
    };

    // IIC_Wire类错误反馈
    enum IIC_Wire_Error
    {
        IIC_WIRE_ERROR_NONE,            //执行成功
        IIC_WIRE_ERROR_UNINITED,        //未正确初始化
        IIC_WIRE_ERROR_NO_BUFFER,       //无缓冲区
        IIC_WIRE_ERROR_BUFFER_SHORTAGE, //缓冲区不足
        IIC_WIRE_ERROR_TARGET_NACK,     //目标无回应
    };

    //抽象GPIO封装类
    class abs_GPIO_Wire
    {
    public:
        /**
         *   设置引脚输出模式
         *   @param Mode -引脚模式,
         *   详见GPIO_Mode枚举类型
         *   @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error setAs(GPIO_Wire_Mode Mode) = 0;
        /**
         *   输出电平
         *   @param Level -决定输出的电平高低
         *   详见GPIO_Level枚举类型
         *   @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error write(GPIO_Wire_Level Level) = 0;

        /**
         *   读取电平高低
         *   @return GPIO_Wire_Level 电平高低
         */
        virtual GPIO_Wire_Level readLevel() = 0;

        /**
         *   获取当前模式
         *   @return GPIO_Wire_Mode 引脚模式
         */
        virtual GPIO_Wire_Mode getMode() = 0;

        /**
         * 翻转电平
         * @return GPIO_Wire_Error 异常抛出
         */
        virtual GPIO_Wire_Error toggle() = 0;
    };

    //抽象IIC协议类
    class abs_IIC_Wire
    {
    public:
        /**
         *   指定目标地址(7位)
         *   @param Aim_addr 目标地址
         *   @return IIC_Wire_Error异常抛出
         */
        virtual IIC_Wire_Error focus_on(uint8_t Aim_addr) = 0;
        /**
         *   设定速度阈值
         *   @param speed_threshold 速度阈值
         */
        virtual void speedLimit(int speed_threshold) = 0;

        /**
         * 设置数据缓冲区，用于接收请求的数据
         * @param lpBuffer 传入缓冲区地址
         * @param buffer_size 缓冲区大小
         */
        virtual void setBuffer(uint8_t *lpBuffer, int buffer_size) = 0;

        /**
         *   写入数据，若第一次发起则发送起始信号
         *   之后发起则继续发送
         *   @param data 写入的数据
         *   @return IIC_Wire_Error异常抛出
         */
        virtual IIC_Wire_Error write(uint8_t data) = 0;
        /**
         *   请求数据，自动发送起始信号并请求count组数据后结束，请求内容应填充到buffer
         *   @param count 连续读取的数据量
         *   @return IIC_Wire_Error异常抛出
         */
        virtual IIC_Wire_Error request(uint8_t count) = 0;
        /**
         *   从buffer中读取请求的数据
         *   @return 请求的值
         */
        virtual uint8_t read() = 0;

        //结束通信
        virtual void end() = 0;

        //带应答结束通信
        virtual void end_ACK() = 0;

        //带非应答的结束通信
        virtual void end_NACK() = 0;
    };

}

#endif
#ifndef __ABS_IIC__
#define __ABS_IIC__

namespace cus
{
    // IIC类错误反馈
    enum IIC_Error
    {
        IIC_ERROR_NONE,            //执行成功
        IIC_ERROR_UNINITED,        //未正确初始化
        IIC_ERROR_NO_BUFFER,       //无缓冲区
        IIC_ERROR_BUFFER_SHORTAGE, //缓冲区不足
        IIC_ERROR_TARGET_NACK,     //目标无回应
    };

    //抽象IIC协议类
    class Abs_IIC_Master
    {
    public:
        /**
         *   指定目标地址(7位)
         *   @param Aim_addr 目标地址
         *   @return IIC_Error异常抛出
         */
        virtual IIC_Error focus_on(uint8_t Aim_addr) = 0;
        /**
         *   设定速度阈值
         *   @param speed_threshold 速度阈值
         */
        virtual void set_SpeedThreshold(int speed_threshold) = 0;

        /**
         *   写入数据，若第一次发起则发送起始信号
         *   之后发起则继续发送
         *   @param data 写入的数据
         *   @return IIC_Error异常抛出
         */
        virtual IIC_Error write(uint8_t data) = 0;
        /**
         *   请求数据，自动发送起始信号并请求count组数据后结束，请求内容应填充到buffer
         *   @param count 连续读取的数据量
         *   @return IIC_Error异常抛出
         */
        virtual IIC_Error request(uint8_t count) = 0;
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
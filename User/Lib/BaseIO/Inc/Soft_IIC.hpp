#ifndef __CUS__SOFT_IIC__
#define __CUS__SOFT_IIC__

#include "Abs_BaseIO.hpp"

namespace cus
{
    //软模拟IIC信号实现
    class Soft_IIC_Master : public Abs_IIC_Master
    {
    protected:
        bool isInit_already;     //是否初正确构造
        Abs_GPIO_Wire *SCL_Wire; //时钟信号对象
        Abs_GPIO_Wire *SDA_Wire; //数据信号对象

        int speed_threshold; //速度阈值，IIC信号跳变间隔

        uint8_t Aim_addr; //目标地址
        bool start_sent;  //判断起始信息是否已经发出

        uint8_t *buffer;     //缓冲区指针
        uint8_t data_length; //缓存区内数据长度
        int buffer_size;     //缓冲区长度

        //引脚操作内联
        inline void SDA_H() { SDA_Wire->write(GPIO_WIRE_LEVEL_HIGH); };
        inline void SDA_L() { SDA_Wire->write(GPIO_WIRE_LEVEL_LOW); };
        inline void SDA_IN() { SDA_Wire->setAs(GPIO_WIRE_MODE_INPUT_PULL_UP); };
        inline void SDA_OUT() { SDA_Wire->setAs(GPIO_WIRE_MODE_OUTPUT_PP); };
        inline void SCL_H() { SCL_Wire->write(cus::GPIO_WIRE_LEVEL_HIGH); };
        inline void SCL_L() { SCL_Wire->write(cus::GPIO_WIRE_LEVEL_LOW); };
        inline void SCL_IN() { SCL_Wire->setAs(GPIO_WIRE_MODE_INPUT_PULL_UP); }
        inline void SCL_OUT() { SCL_Wire->setAs(GPIO_WIRE_MODE_OUTPUT_PP); };
        inline GPIO_Wire_Level SDA_CHECK() { return SDA_Wire->readLevel(); };
        inline GPIO_Wire_Level SCL_CHECK() { return SCL_Wire->readLevel(); };

        // IIC起始信号
        virtual void send_start();
        // IIC结束信号
        virtual void send_stop();
        // IIC应答信号
        virtual void send_ACK();
        // IIC非应答信号
        virtual void send_NACK();

        /**
         * IIC检查应答
         * @return 应答与否
         */
        virtual bool check_ACK();

        /**
         * IIC发送一个字节数据
         * @param data 发送的字节数据
         */
        virtual void writeByte(uint8_t data);

        /**
         * IIC协议内读取一个字节数据
         * @return 读取的字节数据
         */
        virtual uint8_t receiveByte();

        /**
         * 信号时延
         * @param times 延迟的时常
         */
        virtual void delay(int times);

    public:
        //默认构造函数
        Soft_IIC_Master();
        /**
         * 带参构造函数
         * @param SCL_Wire 时钟信号线
         * @param SDA_Wire 数据信号线
         */
        Soft_IIC_Master(Abs_GPIO_Wire &SCL_Wire, Abs_GPIO_Wire &SDA_Wire);

        /**
         * 构造&初始化
         * @param SCL_Wire 时钟信号线
         * @param SDA_Wire 数据信号线
         * @return abs_IIC_Wire实例
         */
        virtual Soft_IIC_Master &construct(Abs_GPIO_Wire &SCL_Wire, Abs_GPIO_Wire &SDA_Wire);

        /**
         * 指定目标地址(7位)
         * @param Aim_addr 目标地址
         * @return IIC_Wire_Error异常抛出
         */
        virtual IIC_Error focus_on(uint8_t Aim_addr);

        /**
         * 设定速度阈值
         * @param speed_threshold 速度阈值
         */
        virtual void set_SpeedThreshold(int speed_threshold);

        /**
         * 写入数据，若第一次发起则发送起始信号
         * 若之后发起则继续发送
         * @param data 写入的数据
         * @return IIC_Wire_Error异常抛出
         */
        virtual IIC_Error write(uint8_t data);

        /**
         * 设置数据缓冲区，用于接收请求的数据
         * @param lpBuffer 传入缓冲区地址
         * @param buffer_size 缓冲区大小
         */
        virtual void setBuffer(uint8_t *lpBuffer, int buffer_size);

        /**
         * 请求数据，自动发送起始信号并请求count组数据后结束，请求内容应填充到buffer
         * @param count 连续读取的数据量
         * @return IIC_Wire_Error异常抛出
         */
        virtual IIC_Error request(uint8_t count);

        /**
         * 从buffer中读取请求的数据
         * @return 请求的值
         */
        virtual uint8_t read();

        //结束通信
        virtual void end();

        //带应答结束通信
        virtual void end_ACK();

        //带非应答的结束通信
        virtual void end_NACK();
    };
}

#endif
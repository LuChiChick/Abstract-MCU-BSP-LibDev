#include "Soft_IIC_Wire.hpp"
#include "stddef.h"

namespace cus
{

    Soft_IIC_Wire::Soft_IIC_Wire()
    {
        this->isInit_already = false;
    }

    Soft_IIC_Wire::Soft_IIC_Wire(abs_GPIO_Wire &SCL_Wire, abs_GPIO_Wire &SDA_Wire)
    {
        construct(SCL_Wire, SDA_Wire);
    }

    Soft_IIC_Wire &Soft_IIC_Wire::construct(abs_GPIO_Wire &SCL_Wire, abs_GPIO_Wire &SDA_Wire)
    {

        //记录引脚
        this->SCL_Wire = &SCL_Wire;
        this->SDA_Wire = &SDA_Wire;

        //模式初始化
        SCL_OUT();
        SDA_OUT();

        //值初始化
        start_sent = false;
        buffer = NULL;
        buffer_size = 0;
        data_length = 0;
        Aim_addr = 0x00;
        //信号稳定时延
        speed_threshold = 20;

        //双线拉高
        SCL_H();
        SDA_H();

        //初始化结束
        this->isInit_already = true;

        //返回构造对象
        return *this;
    }

    IIC_Wire_Error Soft_IIC_Wire::focus_on(uint8_t Aim_addr)
    {
        //初始化审查
        if (!isInit_already)
            return IIC_WIRE_ERROR_UNINITED;
        //地址记录
        this->Aim_addr = Aim_addr;
        //设备应答检测
        {
            send_start();             //起始信号
            writeByte(Aim_addr << 1); //地址+写入位0
            if (check_ACK() == false) //未应答
            {
                send_stop();
                return IIC_WIRE_ERROR_TARGET_NACK;
            }
        }
        //正常应答
        send_stop();
        return IIC_WIRE_ERROR_NONE;
    }

    void Soft_IIC_Wire::speedLimit(int speed_threshold)
    {
        this->speed_threshold = speed_threshold;
    }

    IIC_Wire_Error Soft_IIC_Wire::write(uint8_t data)
    {
        //检查是否初始化
        if (isInit_already == false)
            return IIC_WIRE_ERROR_UNINITED;
        //检查是否第一次发信
        if (start_sent == false)
        {
            send_start();
            //写入7位地址+写入位
            writeByte(Aim_addr << 1);
            //检查应答
            if (check_ACK() == false)
            {
                send_stop();
                return IIC_WIRE_ERROR_TARGET_NACK;
            }
        }
        //写入数据
        writeByte(data);
        //检查应答
        if (check_ACK() == false)
        {
            send_stop();
            return IIC_WIRE_ERROR_TARGET_NACK;
        }
        return IIC_WIRE_ERROR_NONE;
    }

    /**
     * 设置数据缓冲区，用于接收请求的数据
     * @param lpBuffer 传入缓冲区地址
     * @param buffer_size 缓冲区大小
     */
    void Soft_IIC_Wire::setBuffer(uint8_t *lpBuffer, int buffer_size)
    {
        this->buffer = lpBuffer;
        this->buffer_size = buffer_size;
    }

    IIC_Wire_Error Soft_IIC_Wire::request(uint8_t count)
    {
        //检查是否初始化
        if (isInit_already == false)
            return IIC_WIRE_ERROR_UNINITED;
        //检查缓冲区是否溢出，这里顺便检查了缓冲区是否存在
        if (data_length + count > buffer_size)
            return IIC_WIRE_ERROR_BUFFER_SHORTAGE;
        //起始信号
        send_start();
        //写入7位地址+读取位
        writeByte((Aim_addr << 1) + 1);
        //检查应答
        if (check_ACK() == false)
        {
            send_stop();
            return IIC_WIRE_ERROR_TARGET_NACK;
        }
        //循环读取
        for (int c = 0; c < count; c++)
        {
            //存入数据
            buffer[data_length] = receiveByte();
            //更新长度
            data_length++;
            //应答或者结束
            send_ACK();
        }
        end_NACK();
        return IIC_WIRE_ERROR_NONE;
    }

    //读取字节
    uint8_t Soft_IIC_Wire::receiveByte()
    {
        SCL_L();

        //更改为读取模式
        SDA_IN();
        uint8_t data = 0;
        //循环读取
        for (int count = 0; count < 8; count++)
        {
            data <<= 1;
            SCL_H();
            delay(speed_threshold);
            if (SDA_CHECK() == GPIO_WIRE_LEVEL_HIGH)
            {
                data++;
            }
            //拉低时钟保持稳定
            SCL_L();
            delay(speed_threshold);
        }
        //恢复输出模式
        SDA_OUT();
        return data;
    }

    uint8_t Soft_IIC_Wire::read()
    {
        if (data_length != 0)
        {
            uint8_t dataReturn = buffer[0];
            //移动数据
            for (int count = 1; count < buffer_size; count++)
                buffer[count - 1] = buffer[count];
            //数据指针-1
            data_length--;
            return dataReturn;
        }
        return 0;
    }

    void Soft_IIC_Wire::end()
    {
        send_stop();
    }

    void Soft_IIC_Wire::delay(int times)
    {
        //循环时延
        for (int count = 0; count < times; count++)
            ;
    }

    //带应答结束通信
    void Soft_IIC_Wire::end_ACK()
    {
        send_ACK();
        send_stop();
    }

    //带非应答的结束通信
    void Soft_IIC_Wire::end_NACK()
    {
        send_NACK();
        send_stop();
    }

    // IIC起始信号
    void Soft_IIC_Wire::send_start()
    {
        //双线拉高
        SDA_H();
        SCL_H();
        //稳定时延
        delay(speed_threshold);
        //时钟线高时数据线由高转低，发送起始信号
        SDA_L();
        //稳定时延
        delay(speed_threshold);
        //拉低时钟线线进行接下来的动作
        SCL_L();
        //稳定时延
        delay(speed_threshold);
        //记录起始信号发送
        start_sent = true;
    }

    void Soft_IIC_Wire::send_stop()
    {
        //执行前时钟线务必是低
        SCL_L();
        SDA_L();
        //稳定时延
        delay(speed_threshold);
        //拉高时钟信号
        SCL_H();
        //稳定时延
        delay(speed_threshold);
        //时钟线高时数据线低转高，发送结束信号
        SDA_H();
        //稳定时延
        delay(speed_threshold);
        //结束后SDA与SCL同为高，不占用总线

        //记录起始信号未发送
        start_sent = false;
    }

    void Soft_IIC_Wire::send_ACK()
    {
        //拉低SDA
        SDA_L();
        //稳定时延
        delay(speed_threshold);
        // SCL脉冲
        SCL_H();
        delay(speed_threshold);
        SCL_L();
        delay(speed_threshold);
    }

    void Soft_IIC_Wire::send_NACK()
    {
        //拉高SDA
        SDA_H();
        //稳定时延
        delay(speed_threshold);
        // SCL脉冲
        SCL_H();
        delay(speed_threshold);
        SCL_L();
        delay(speed_threshold);
    }

    bool Soft_IIC_Wire::check_ACK()
    {
        // SDA输入模式
        SDA_IN();
        //稳定时延
        delay(speed_threshold);
        //拉高SCL
        SCL_H();

        //循环检查ACK信号20个信号稳定周期
        bool condition = false;
        for (int count = 0; count < 20 * speed_threshold; count++)
        {
            //读取到低电平为ACK
            if (SDA_CHECK() == GPIO_WIRE_LEVEL_LOW)
            {
                //修改contition
                condition = true;
                break;
            }
        }
        //拉低时钟信号
        SCL_L();
        //稳定时延
        delay(speed_threshold);
        //更新模式
        SDA_OUT();
        return condition;
    }

    void Soft_IIC_Wire::writeByte(uint8_t data)
    {
        //循环写入，由高位到低位
        for (int count = 7; count >= 0; count--)
        {
            //移位字节信号
            if ((data >> count) & 0x1)
                SDA_H();
            else
                SDA_L();
            //稳定SDA信号
            delay(speed_threshold);
            // SCL脉冲
            SCL_H();
            delay(speed_threshold);
            SCL_L();
            delay(speed_threshold);
        }
    }

}
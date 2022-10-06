#include "MSP432_GPIO_Wire.hpp"

namespace cus
{
    //默认构造函数
    MSP432_GPIO_Wire::MSP432_GPIO_Wire()
    {
        isInit_already = false;
    }

    /**
     * 带参构造函数
     * @param GPIOx 引脚区域
     * @param GPIO_Pin 引脚编号
     */
    MSP432_GPIO_Wire::MSP432_GPIO_Wire(uint_fast8_t GPIOx, uint_fast16_t GPIO_Pin)
    {
        construct(GPIOx, GPIO_Pin);
    }

    /**
     * 构造&类初始化
     * @param GPIOx 引脚区域
     * @param GPIO_Pin 引脚编号
     * @return abs_Wire实例
     */
    MSP432_GPIO_Wire &MSP432_GPIO_Wire::construct(uint_fast8_t GPIOx, uint_fast16_t GPIO_Pin)
    {
        //信息记录
        this->GPIOx = GPIOx;
        this->GPIO_Pin = GPIO_Pin;
        //模式初始记录
        this->Mode = GPIO_WIRE_MODE_UNKNOWN;
        //正确构造
        this->isInit_already = true;
        //返回构造对象
        return *this;
    }

    /**
     * 设置引脚输出模式
     * @param Mode -引脚模式,
     * 详见GPIO_Wire_Mode枚举类型
     * @return GPIO_Wire_Error 异常抛出
     */
    GPIO_Wire_Error MSP432_GPIO_Wire::setAs(GPIO_Wire_Mode Mode)
    {
        //异常初始化排除
        if (this->isInit_already != true)
            return GPIO_WIRE_ERROR_UNINITED;

        //匹配模式处理
        switch (Mode)
        {
        case GPIO_WIRE_MODE_OUTPUT_PP: //推免输出模式
        case GPIO_WIRE_MODE_OUTPUT_OD: //开漏输出模式

            // MSP432 DriverLib 未提供开漏输出，使用推挽输出代替
            GPIO_setAsOutputPin(GPIOx, GPIO_Pin);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        case GPIO_WIRE_MODE_INPUT_PULL_UP: //拉高输入模式

            GPIO_setAsInputPinWithPullUpResistor(GPIOx, GPIO_Pin);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        case GPIO_WIRE_MODE_INPUT_PULL_DOWN: //拉低输入模式

            GPIO_setAsInputPinWithPullDownResistor(GPIOx, GPIO_Pin);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        case GPIO_WIRE_MODE_INPUT_NO_PULL: //普通无上下拉输入模式

            GPIO_setAsInputPin(GPIOx, GPIO_Pin);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        default: //未受检索的模式

            //异常记录
            this->Mode = GPIO_WIRE_MODE_UNKNOWN;
            //抛出异常
            return GPIO_WIRE_ERROR_MODE_UNKNOWN;
        }
    }

    /**
     * 输出电平
     * @param Level -决定输出的电平高低
     * 详见GPIO_Wire_Level枚举类型
     * @return GPIO_Wire_Error 异常抛出
     */
    GPIO_Wire_Error MSP432_GPIO_Wire::write(GPIO_Wire_Level Level)
    {
        //异常初始化排除
        if (this->isInit_already != true)
            return GPIO_WIRE_ERROR_UNINITED;

        //匹配模式处理
        switch (this->Mode)
        {
        case GPIO_WIRE_MODE_OUTPUT_PP: //推免输出模式
        case GPIO_WIRE_MODE_OUTPUT_OD: //开漏输出模式

            //匹配电平模式输出
            if (Level == GPIO_WIRE_LEVEL_HIGH)
            {
                GPIO_setOutputHighOnPin(GPIOx, GPIO_Pin);
                return GPIO_WIRE_ERROR_NONE;
            }
            if (Level == GPIO_WIRE_LEVEL_LOW)
            {
                GPIO_setOutputLowOnPin(GPIOx, GPIO_Pin);
                return GPIO_WIRE_ERROR_NONE;
            }
            else
                //写入电平未知
                return GPIO_WIRE_ERROR_LEVEL_UNKNOWN;
        default:
            //未匹配任何模式
            return GPIO_WIRE_ERROR_MODE_INCORRECT;
        }
    }

    /**
     * 读取电平高低
     * @return GPIO_Wire_Level 电平高低
     */
    GPIO_Wire_Level MSP432_GPIO_Wire::readLevel()
    {
        //异常构造排除
        if (this->isInit_already != true)
            return GPIO_WIRE_LEVEL_UNKNOWN;

        switch (this->Mode)
        {
        case GPIO_WIRE_MODE_INPUT_PULL_UP:
        case GPIO_WIRE_MODE_INPUT_PULL_DOWN:
        case GPIO_WIRE_MODE_INPUT_NO_PULL:

            //读取并转换输出电平
            if ((GPIO_Wire_Level)GPIO_getInputPinValue(GPIOx, GPIO_Pin) == GPIO_WIRE_LEVEL_HIGH)
                return GPIO_WIRE_LEVEL_HIGH;
            else
                return GPIO_WIRE_LEVEL_LOW;
        default:
            return GPIO_WIRE_LEVEL_UNKNOWN;
        }
    }

    /**
     * 获取当前模式
     * @return GPIO_Wire_Mode 引脚模式
     */
    GPIO_Wire_Mode MSP432_GPIO_Wire::getMode()
    {
        return this->Mode;
    }

    /**
     * 翻转电平
     * @return GPIO_Wire_Error 异常抛出
     */
    GPIO_Wire_Error MSP432_GPIO_Wire::toggle()
    {
        //异常构造排除
        if (this->isInit_already != true)
            return GPIO_WIRE_ERROR_UNINITED;
        //模式筛查
        switch (this->Mode)
        {
        case GPIO_WIRE_MODE_OUTPUT_OD:
        case GPIO_WIRE_MODE_OUTPUT_PP:

            //切换引脚电平
            GPIO_toggleOutputOnPin(GPIOx, GPIO_Pin);

            break;

        default:
            return GPIO_WIRE_ERROR_MODE_INCORRECT;
        }
        return GPIO_WIRE_ERROR_NONE;
    }

    //释放资源
    GPIO_Wire_Error MSP432_GPIO_Wire::release()
    {
        if (this->isInit_already)
        {
            //引脚模式记录
            this->Mode = GPIO_WIRE_MODE_UNKNOWN;
            return GPIO_WIRE_ERROR_NONE;
        }
        else
            return GPIO_WIRE_ERROR_UNINITED;
    }
}
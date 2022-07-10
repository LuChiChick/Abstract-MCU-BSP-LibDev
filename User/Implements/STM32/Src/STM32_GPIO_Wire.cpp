#include "STM32_GPIO_Wire.hpp"

namespace cus
{
    STM32_GPIO_Wire::STM32_GPIO_Wire()
    {
        isInit_already = false;
    }

    STM32_GPIO_Wire::STM32_GPIO_Wire(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
    {
        construct(GPIOx, GPIO_Pin);
    }

    STM32_GPIO_Wire &STM32_GPIO_Wire::construct(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
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

    GPIO_Wire_Error STM32_GPIO_Wire::setAs(GPIO_Wire_Mode Mode)
    {
        //异常初始化排除
        if (this->isInit_already != true)
            return GPIO_WIRE_ERROR_UNINITED;
        //解应用
        release();
        //设置结构体
        static GPIO_InitTypeDef pinSet = {0};
        //指定引脚
        pinSet.Pin = GPIO_Pin;
        //快速toggle
        pinSet.Speed = GPIO_SPEED_HIGH;
        //匹配模式处理
        switch (Mode)
        {
        case GPIO_WIRE_MODE_OUTPUT_PP: //推免输出模式

            //推免输出
            pinSet.Mode = GPIO_MODE_OUTPUT_PP;
            //无上拉
            pinSet.Pull = GPIO_NOPULL;
            //初始化
            HAL_GPIO_Init(GPIOx, &pinSet);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        case GPIO_WIRE_MODE_OUTPUT_OD: //开漏输出模式

            //推免输出
            pinSet.Mode = GPIO_MODE_OUTPUT_OD;
            //无上拉
            pinSet.Pull = GPIO_NOPULL;
            //初始化
            HAL_GPIO_Init(GPIOx, &pinSet);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        case GPIO_WIRE_MODE_INPUT_PULL_UP: //拉高输入模式

            //输入模式
            pinSet.Mode = GPIO_MODE_INPUT;
            //上拉
            pinSet.Pull = GPIO_PULLUP;
            //初始化
            HAL_GPIO_Init(GPIOx, &pinSet);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        case GPIO_WIRE_MODE_INPUT_PULL_DOWN: //拉低输入模式

            //输入模式
            pinSet.Mode = GPIO_MODE_INPUT;
            //上拉
            pinSet.Pull = GPIO_PULLDOWN;
            //初始化
            HAL_GPIO_Init(GPIOx, &pinSet);
            //信息记录
            this->Mode = Mode;
            //返回
            return GPIO_WIRE_ERROR_NONE;

        case GPIO_WIRE_MODE_INPUT_NO_PULL: //普通无上下拉输入模式

            //输入模式
            pinSet.Mode = GPIO_MODE_INPUT;
            //上拉
            pinSet.Pull = GPIO_NOPULL;
            //初始化
            HAL_GPIO_Init(GPIOx, &pinSet);
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

    GPIO_Wire_Error STM32_GPIO_Wire::write(GPIO_Wire_Level Level)
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
                HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
                return GPIO_WIRE_ERROR_NONE;
            }
            if (Level == GPIO_WIRE_LEVEL_LOW)
            {
                HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
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

    GPIO_Wire_Level STM32_GPIO_Wire::readLevel()
    {
        //异常构造排除
        if (this->isInit_already != true)
            return GPIO_WIRE_LEVEL_UNKNOWN;

        switch (this->Mode)
        {
        case GPIO_WIRE_MODE_INPUT_PULL_UP:
        case GPIO_WIRE_MODE_INPUT_PULL_DOWN:
        case GPIO_WIRE_MODE_INPUT_NO_PULL:
            if ((GPIO_Wire_Level)HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_WIRE_LEVEL_HIGH)
                return GPIO_WIRE_LEVEL_HIGH;
            else
                return GPIO_WIRE_LEVEL_LOW;
        default:
            return GPIO_WIRE_LEVEL_UNKNOWN;
        }
    }

    GPIO_Wire_Mode STM32_GPIO_Wire::getMode()
    {
        return this->Mode;
    }

    GPIO_Wire_Error STM32_GPIO_Wire::toggle()
    {
        //异常构造排除
        if (this->isInit_already != true)
            return GPIO_WIRE_ERROR_UNINITED;
        //模式筛查
        switch (this->Mode)
        {
        case GPIO_WIRE_MODE_OUTPUT_OD:
        case GPIO_WIRE_MODE_OUTPUT_PP:
            HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
            break;
        default:
            return GPIO_WIRE_ERROR_MODE_INCORRECT;
        }
        return GPIO_WIRE_ERROR_NONE;
    }

    GPIO_Wire_Error STM32_GPIO_Wire::release()
    {
        if (this->isInit_already)
        {
            HAL_GPIO_DeInit(GPIOx, GPIO_Pin);
            this->Mode = GPIO_WIRE_MODE_UNKNOWN;
            return GPIO_WIRE_ERROR_NONE;
        }
        else
            return GPIO_WIRE_ERROR_UNINITED;
    }
}
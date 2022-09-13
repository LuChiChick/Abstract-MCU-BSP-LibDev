#ifndef __CUS__MSP432_SYSTEM_SETTINGS__
#define __CUS__MSP432_SYSTEM_SETTINGS__

// MSP432 DriverLib包含
extern "C"
{
#include "driverlib.h"
}

namespace cus
{

    /**
     * 修改系统运行频率
     * @param frequency 指定频率
     * @arg CS_15MHZ
     * @arg CS_3MHZ
     * @arg CS_12MHZ
     * @arg CS_24MHZ
     * @arg CS_48MHZ
     */
    void setSystemFrequency(uint32_t frequency);

}
#endif

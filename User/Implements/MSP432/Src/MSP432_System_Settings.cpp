#include "MSP432_System_Settings.hpp"

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
    void setSystemFrequency(uint32_t frequency)
    {
        //前置操作
        {
// Enable FPU if used
#if (__FPU_USED == 1)                        // __FPU_USED is defined in core_cm4.h
            SCB->CPACR |= ((3UL << 10 * 2) | // Set CP10 Full Access
                           (3UL << 11 * 2)); // Set CP11 Full Access
#endif

#if (__HALT_WDT == 1)
            WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Halt the WDT
#endif

            SYSCTL->SRAM_BANKEN = SYSCTL_SRAM_BANKEN_BNK7_EN; // Enable all SRAM banks
        }

        //筛选频率
        switch (frequency)
        {
        case CS_15MHZ: // 1.5MHZ
        {
// Default VCORE is LDO VCORE0 so no change necessary

// Switches LDO VCORE0 to DCDC VCORE0 if requested
#if __REGULATOR
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_4;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
#endif

            // No flash wait states necessary

            // DCO = 1.5 MHz; MCLK = source
            CS->KEY = CS_KEY_VAL;         // Unlock CS module for register access
            CS->CTL0 = CS_CTL0_DCORSEL_0; // Set DCO to 1.5MHz
            CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK)) | CS_CTL1_SELM__DCOCLK;
            // Select MCLK as DCO source
            CS->KEY = 0;

            // Set Flash Bank read buffering
            FLCTL->BANK0_RDCTL = FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_BUFD | FLCTL_BANK0_RDCTL_BUFI);
            FLCTL->BANK1_RDCTL = FLCTL->BANK1_RDCTL & ~(FLCTL_BANK1_RDCTL_BUFD | FLCTL_BANK1_RDCTL_BUFI);

            //更新全局频率
            SystemCoreClock = frequency;
            break;
        }
        case CS_3MHZ: // 3MHZ
        {
            // Default VCORE is LDO VCORE0 so no change necessary

// Switches LDO VCORE0 to DCDC VCORE0 if requested
#if __REGULATOR
            while (PCM->CTL1 & PCM_CTL1_PMR_BUSY)
                ;
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_4;
            while (PCM->CTL1 & PCM_CTL1_PMR_BUSY)
                ;
#endif

            // No flash wait states necessary

            // DCO = 3 MHz; MCLK = source
            CS->KEY = CS_KEY_VAL;         // Unlock CS module for register access
            CS->CTL0 = CS_CTL0_DCORSEL_1; // Set DCO to 1.5MHz
            CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK)) | CS_CTL1_SELM__DCOCLK;
            // Select MCLK as DCO source
            CS->KEY = 0;

            // Set Flash Bank read buffering
            FLCTL->BANK0_RDCTL = FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_BUFD | FLCTL_BANK0_RDCTL_BUFI);
            FLCTL->BANK1_RDCTL = FLCTL->BANK1_RDCTL & ~(FLCTL_BANK1_RDCTL_BUFD | FLCTL_BANK1_RDCTL_BUFI);

            //更新全局频率
            SystemCoreClock = frequency;
            break;
        }
        case CS_12MHZ: // 12MHZ
        {
            // Default VCORE is LDO VCORE0 so no change necessary

// Switches LDO VCORE0 to DCDC VCORE0 if requested
#if __REGULATOR
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_4;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
#endif

            // No flash wait states necessary

            // DCO = 12 MHz; MCLK = source
            CS->KEY = CS_KEY_VAL;         // Unlock CS module for register access
            CS->CTL0 = CS_CTL0_DCORSEL_3; // Set DCO to 12MHz
            CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK)) | CS_CTL1_SELM__DCOCLK;
            // Select MCLK as DCO source
            CS->KEY = 0;

            // Set Flash Bank read buffering
            FLCTL->BANK0_RDCTL = FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_BUFD | FLCTL_BANK0_RDCTL_BUFI);
            FLCTL->BANK1_RDCTL = FLCTL->BANK1_RDCTL & ~(FLCTL_BANK1_RDCTL_BUFD | FLCTL_BANK1_RDCTL_BUFI);

            //更新全局频率
            SystemCoreClock = frequency;
            break;
        }
        case CS_24MHZ: // 24MHZ
        {
            // Default VCORE is LDO VCORE0 so no change necessary

// Switches LDO VCORE0 to DCDC VCORE0 if requested
#if __REGULATOR
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_4;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
#endif

            // 1 flash wait state (BANK0 VCORE0 max is 12 MHz)
            FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~FLCTL_BANK0_RDCTL_WAIT_MASK) | FLCTL_BANK0_RDCTL_WAIT_1;
            FLCTL->BANK1_RDCTL = (FLCTL->BANK1_RDCTL & ~FLCTL_BANK1_RDCTL_WAIT_MASK) | FLCTL_BANK1_RDCTL_WAIT_1;

            // DCO = 24 MHz; MCLK = source
            CS->KEY = CS_KEY_VAL;         // Unlock CS module for register access
            CS->CTL0 = CS_CTL0_DCORSEL_4; // Set DCO to 24MHz
            CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK)) | CS_CTL1_SELM__DCOCLK;
            // Select MCLK as DCO source
            CS->KEY = 0;

            // Set Flash Bank read buffering
            FLCTL->BANK0_RDCTL = FLCTL->BANK0_RDCTL | (FLCTL_BANK0_RDCTL_BUFD | FLCTL_BANK0_RDCTL_BUFI);
            FLCTL->BANK1_RDCTL = FLCTL->BANK1_RDCTL & ~(FLCTL_BANK1_RDCTL_BUFD | FLCTL_BANK1_RDCTL_BUFI);

            //更新全局频率
            SystemCoreClock = frequency;
            break;
        }
        case CS_48MHZ: // 48MHZ
        {
            // Switches LDO VCORE0 to LDO VCORE1; mandatory for 48 MHz setting
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;

// Switches LDO VCORE1 to DCDC VCORE1 if requested
#if __REGULATOR
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
            PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_5;
            while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY))
                ;
#endif

            // 1 flash wait states (BANK0 VCORE1 max is 16 MHz, BANK1 VCORE1 max is 32 MHz)
            FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~FLCTL_BANK0_RDCTL_WAIT_MASK) | FLCTL_BANK0_RDCTL_WAIT_1;
            FLCTL->BANK1_RDCTL = (FLCTL->BANK1_RDCTL & ~FLCTL_BANK1_RDCTL_WAIT_MASK) | FLCTL_BANK1_RDCTL_WAIT_1;

            // DCO = 48 MHz; MCLK = source
            CS->KEY = CS_KEY_VAL;         // Unlock CS module for register access
            CS->CTL0 = CS_CTL0_DCORSEL_5; // Set DCO to 48MHz
            CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK)) | CS_CTL1_SELM__DCOCLK;
            // Select MCLK as DCO source
            CS->KEY = 0;

            // Set Flash Bank read buffering
            FLCTL->BANK0_RDCTL = FLCTL->BANK0_RDCTL | (FLCTL_BANK0_RDCTL_BUFD | FLCTL_BANK0_RDCTL_BUFI);
            FLCTL->BANK1_RDCTL = FLCTL->BANK1_RDCTL | (FLCTL_BANK1_RDCTL_BUFD | FLCTL_BANK1_RDCTL_BUFI);

            //更新全局频率
            SystemCoreClock = frequency;
            break;
        }
        default: //其余情况
            return;
        }
    }
}

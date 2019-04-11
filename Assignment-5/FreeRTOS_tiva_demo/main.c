/*
 *@filename      : main.c
 *@author        : Puneet Bansal
 *@description   : Program to create 3 tasks, tempTask, alertTask and ledTask and start the scheduler.
 *@reference     : Andy Kobyljanec FreeRTOS demo : https://github.com/akobyl/TM4C129_FreeRTOS_Demo
 *                 FreeRTOS website : https://www.freertos.org/
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "ledTask.h"
#include "loggerTask.h"
#include "tempTask.h"
#include "alertTask.h"
#include "i2c.h"


int main(void)
{
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
            (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                    SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                    SYSTEM_CLOCK);

    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);
    UARTprintf("\nUART CHECK\n");
    i2cInit();
    I2CMasterInitExpClk(I2C0_BASE,output_clock_rate_hz, false);

    if(ledTaskInit()!=0)
    {
        UARTprintf("\nledTask creation failed\n");
    }
    else
    {
        UARTprintf("\nledTask creation successful\n");
    }

    if(loggerTaskInit()!=0)
    {
        UARTprintf("\nloggerTask creation failed\n");
    }
    else
    {
        UARTprintf("\nloggerTask creation successful\n");
    }
    if(tempTaskInit()!=0)
    {
        UARTprintf("\ntempTask creation failed\n");
    }
    else
    {
        UARTprintf("\ntempTask creation successful\n");
    }
    if(alertTaskInit()!=0)
    {
        UARTprintf("\nalertTask creation failed\n");
    }
    else
    {
        UARTprintf("\nalertTask creation successful\n");
    }


    vTaskStartScheduler();

    while(1)
    {


    }
    return 0;
}

void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}

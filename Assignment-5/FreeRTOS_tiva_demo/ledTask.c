/*
 *@filename      : ledTask.c
 *author         : Puneet Bansal
 *description    : Consists functions to generate LED Task, generation of LED timer and toggling the LED at
                  a frequency of 10Hz.Details of all the functions is provided in the ledTask.h file
 */

#include "ledTask.h"
#include "driverlib/uart.h"

xTaskHandle g_ledHandle;
TimerHandle_t ledTimer;
volatile int timerCount;

void vledTimerCallBackFunction(TimerHandle_t ledTimer )
{
    TickType_t ticks;
    int i;
    i=1;
    ticks=0;
    payload messageToSend;
    timerCount++;
    //UARTprintf("Entered timer handler\n %d",timerCount);
    if(GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_0)==0)
    {
        led0TurnOn();
        led1TurnOff();
    }
    else
    {
        led0TurnOff();
        led1TurnOn();
    }

    ticks=xTaskGetTickCount();
    float ticksToSec=0;
    char printfString[50];

    ticksToSec=(float)ticks/configTICK_RATE_HZ;
    messageToSend.name="Puneet";
    messageToSend.source="led";
    messageToSend.timeStamp=(float)ticksToSec;
    messageToSend.toggleCount=(int)timerCount;

    //Sending the data via message queue to the logger task*/
    if( xQueueSend( loggerQueue, (void *)&messageToSend, 1000) != pdPASS )
    {
        UARTprintf("Failed to post the message, even after 10 ticks\n");
    }
    //printf("sending data via queue successful\n");
    /*******************************************************/
}

void ledTask(void *param)
{
    TickType_t ticks;
    timerCount=0;
    loggerQueue = xQueueCreate( 10, sizeof(payload) );
    //loggerQueue = xQueueCreate( 10, sizeof(int) );

    if(loggerQueue==0)
    {
        UARTprintf("Queue creation failed\n");
    }
    ledInit();
    ledTimer = xTimerCreate("ledTimer1", pdMS_TO_TICKS(100), pdTRUE, (void *)0, vledTimerCallBackFunction);
    if(ledTimer==NULL)
    {
        UARTprintf("Softimer creation FAILED\n");
    }
    else
    {
        UARTprintf("Softimer creation successfull\n");
        if(xTimerStart(ledTimer,100)!=pdPASS)
        {
            UARTprintf("Softimer setting FAILED\n");
        }
    }

    while(1)
    {

    }

}

int ledTaskInit()
{
    if(xTaskCreate(ledTask, (const char *)"ledTask",
                   1024, NULL,1,&g_ledHandle) != pdTRUE)
    {
        // Task creation failed.
        UARTprintf("Task creation failed");
        return(1);
    }

    if(g_ledHandle==NULL)
    {
        UARTprintf("Led Task handle null");
    }

    return 0;
}

void ledInit()
{
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // Enable the GPIO pin for the LED (PN0).  Set the direction as output, and
    // enable the GPIO pin for digital function.

    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
}

void led0TurnOn()
{
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
}

void led0TurnOff()
{
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
}
void led1TurnOn()
{
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
}

void led1TurnOff()
{
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
}




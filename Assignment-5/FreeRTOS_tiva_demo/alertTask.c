/*
 *@filename      : alertTask.c
 *author         : Puneet Bansal
 *description    : Consists functions to generate alert Task and sending alert values to the logger task.
                  Details of all the functions is provided in the ledTask.h file
 */

#include "alertTask.h"
#include "ledTask.h"
#include "tempTask.h"
#include "driverlib/uart.h"

#define ULONG_MAX 0xffffffff

void alertTask(void *param)
{
    TickType_t ticks;
    uint32_t val;
    val=0;
    payload messageToSend;
    messageToSend.source="alert";
    messageToSend.msg="TEMP CROSSED THRESHOLD!!";
    while(1)
    {
        xTaskNotifyWait( 0x00,      /* Don't clear any notification bits on entry. */
                         ULONG_MAX, /* Reset the notification value to 0 on exit. */
                         &val, /* Notified value pass out in
                                                      ulNotifiedValue. */
                         portMAX_DELAY);


        ticks=xTaskGetTickCount();

        float ticksToSec=0;
        char printfString[50];

        ticksToSec=(float)ticks/configTICK_RATE_HZ;
        messageToSend.timeStamp = ticksToSec;

        if( xQueueSend( loggerQueue, (void *)&messageToSend, 1000) != pdPASS )
        {
            UARTprintf("Failed to post the message, even after 10 ticks\n");
        }

    }

}

int alertTaskInit()
{
    if(xTaskCreate(alertTask, (const char *)"alertTask",
                   1024, NULL,1,&g_alertHandle) != pdTRUE)
    {
        //Task creation failed
        UARTprintf("Alert task creation failed\n");
        return(1);
    }

    if(g_alertHandle==NULL)
    {
        //checking alert handle
        UARTprintf("Alert Task handle null\n");
    }

    return 0;
}







/*
 *@filename      : loggerTask.c
 *author         : Puneet Bansal
 *description    : Consists functions to generate logger Task and sending log data to the terminal via
                   UART.Details of all the functions is provided in the ledTask.h file
 */

#include "loggerTask.h"
#include "ledTask.h"
#include "driverlib/uart.h"
#include <string.h>

xTaskHandle g_loggerHandle;

void loggerTask(void *param)
{
    UARTprintf("\nLogger Task Entered\n");
    char printMsg[50];

    payload messageToReceive;
    while(1)
    {
        if( loggerQueue != 0 )
        {
            // Receive a message on the created queue.
            if( xQueueReceive( loggerQueue,(void *)& messageToReceive, 1000 )  )
            {
                if(strcmp(messageToReceive.source,"led")==0)
                {
                    sprintf(printMsg,"[%f] [%s] Toggle count is :%d Name is %s",messageToReceive.timeStamp,messageToReceive.source,messageToReceive.toggleCount,messageToReceive.name);
                    //sprintf(printMsg,"%d",messageToReceive.toggleCount);
                    UARTprintf(printMsg);
                    UARTprintf("\n");
                }
                else if(strcmp(messageToReceive.source,"temp")==0)
                {
                    sprintf(printMsg,"[%f] [%s] Temperature : %f C",messageToReceive.timeStamp,messageToReceive.source,messageToReceive.value);
                    UARTprintf(printMsg);
                    UARTprintf("\n");
                }
                else if(strcmp(messageToReceive.source,"alert")==0)
                {
                    sprintf(printMsg,"[%f] [%s] %s",messageToReceive.timeStamp,messageToReceive.source,messageToReceive.msg);
                    UARTprintf(printMsg);
                    UARTprintf("\n");
                }

            }
        }
    }
}

int loggerTaskInit()
{
    if(xTaskCreate(loggerTask, (const char *)"loggerTask",
                   1024, NULL,1,&g_loggerHandle) != pdTRUE)
    {
        //
        // Task creation failed.
        //
        UARTprintf("Logger task creation failed\n");
        return(1);
    }
    if(g_loggerHandle==NULL)
    {
        UARTprintf("\nLogger task handle null\n");
    }

    return 0;

}






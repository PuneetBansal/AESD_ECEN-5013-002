/*
*@filename      : ledTask.h
*author         : Puneet Bansal
*description    : Contains declaration along with their description of the functions supporting ledTask
*/


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "freeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "projdefs.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"


typedef struct
{
    float timeStamp;
    float value;
    char *source;
    char *name;
    int toggleCount;
    char *msg;
}payload;


QueueHandle_t loggerQueue;
QueueHandle_t alertQueue;
xTaskHandle g_alertHandle;



/***************LED Related User Defined Functions*********************/

/*
 * @name : ledTaskInit
 * @desc : Creates task, and assigns a task handle and task callback function.
 * @return: returns 0 on successful task creation and 1 if task creation failed.
 */
int ledTaskInit();

/*
 *@name: ledTask
 *@desc: creates a softimer which expires every 100 ms.
 */

void ledTask(void *param);

/*
 * @name : vledTimerCallBackFunction
 * @desc : Toggles the Led every time it is invoked and keeps a track of toggle count.Populates the loggerStruct
 *         with the source, toggleCount, timestamp and a name of the person. This information is sent to
 *         loggerTask via FreeRTOS queues.
 */

void vledTimerCallBackFunction(TimerHandle_t);

/*
 * @name : ledInit
 * @desc : Enables the appropriate port, pin and the direction of the GPIO pins to be used as LED's
 */
void ledInit();

/*
 * @name : led0TurnOn();
 * @desc : Wrapper to turn on the LED0 i.e Port N pin 0
 */
void led0TurnOn();

/*
 * @name : led0TurnOff();
 * @desc : Wrapper to turn off the LED0 i.e Port N pin 0
 */
void led0TurnOff();

/*
 * @name : led1TurnOn();
 * @desc : Wrapper to turn on the LED1 i.e Port N pin 1
 */

void led1TurnOn();

/*
 * @name : led1TurnOff();
 * @desc : Wrapper to turn off the LED1 i.e Port N pin 1
 */
void led1TurnOff();
/**********************************************************************/


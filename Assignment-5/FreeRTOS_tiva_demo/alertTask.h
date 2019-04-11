/*
*@filename      : alertTask.h
*author         : Puneet Bansal
*description    : Contains declaration along with their description of the functions supporting alertTask
*/
#ifndef ALERTTASK_H_
#define ALERTTASK_H_


//QueueHandle_t alertQueue;
//xTaskHandle g_alertHandle;

/*
 * @name : alertTaskInit
 * @desc : Creates task, and assigns a task handle and task callback function.
 * @return: returns 0 on successful task creation and 1 if task creation failed.
 */
int alertTaskInit();

/*
 * @name : alertTask
 * @desc : waits for alert from the temp task. As soon as it receives the notification, it sends the alert
 *         message to the logger task along with time stamp and message source.
 *

void alertTask(void *param)
*/

#endif /* ALERTTASK_H_ */

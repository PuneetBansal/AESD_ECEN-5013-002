/*
*@filename      : loggerTask.h
*author         : Puneet Bansal
*description    : Contains declaration along with their description of the functions supporting loggerTask
*/


#ifndef LOGGERTASK_H_
#define LOGGERTASK_H_

/*
 * @name    : loggerTaskInit
 * @desc    : Creates task, and assigns a task handle and task callback function.
 * @return  : returns 0 on successful task creation and 1 if task creation failed.
*/

int loggerTaskInit();

/*
 * @name : loggerTask
 * @desc : Receives a message in logger queue sorts them according to the source and prints the dedicated
 *         message to the terminal via UART.
 */

void loggerTask();

#endif /* LOGGERTASK_H_ */

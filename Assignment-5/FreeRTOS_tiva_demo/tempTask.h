/*
*@filename      : tempTask.h
*author         : Puneet Bansal
*description    : Contains declaration along with their description of the functions supporting tempTask
*/

#include <stdint.h>
#ifndef TEMPTASK_H_
#define TEMPTASK_H_

#define SLAVEADD 0x48
#define TEMP_REG 0x00
#define CONFIG_REG 0X01
#define TLOW_REG 0X02
#define THIGH_REG 0X03

/*
 * @name : i2cInit
 * @desc : Initialises the i2c peripheral, GPIO pins and their alternate functions.
 */
void i2cInit();

/*
 * @name    : tempi2cWrite
 * @param   : regAdd, data
 * @desc    : Writes data specified by the data paramter to a register specified by regAdd
 */
void tempi2cWrite(uint8_t,uint8_t);

/*
 * @name    : tempi2cRead
 * @param   : registerAdd
 * @desc    : Reads data specified from the register specified by regAdd and returns the value after computations.
 * @return  : uint32_t value read from the register.
 */

uint32_t tempi2cRead(uint8_t registerAdd);

/*
 * @name : tempTaskInit
 * @desc : Creates task, and assigns a task handle and task callback function.
 * @return: returns 0 on successful task creation and 1 if task creation failed.
 */
int tempTaskInit();

/*
 * @name : tempTask
 * @desc : Creates a softimer which expires after every 1s.
 */

void tempTask(void *param);


#define SLAVEADD 0x48




#endif /* TEMPTASK_H_ */

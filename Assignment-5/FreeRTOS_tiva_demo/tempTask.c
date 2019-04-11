/*
 *@filename      : tempTask.c
 *author         : Puneet Bansal
 *description    : Consists functions to generate temp Task, generation of temp timer and measuring the temp at
                   a frequency of 1Hz.Details of all the functions is provided in the tempTask.h file
 */

#include "tempTask.h"
#include "ledTask.h"
#include "loggerTask.h"
#include "driverlib/i2c.h"
#include "driverlib/uart.h"
#include "alertTask.h"

xTaskHandle g_tempHandle;
TimerHandle_t tempTimer;

void vtempTimerCallBackFunction(TimerHandle_t tempTimer )
{
    uint32_t readData;
    float tempInC;
    TickType_t ticks;
    payload messageToSend;
    ticks=0;

    /*Timestamp calculation*/
    ticks=xTaskGetTickCount();
    float ticksToSec=0;
    char printfString[50];
    ticksToSec=(float)ticks/configTICK_RATE_HZ;

    readData= tempi2cRead(TEMP_REG);
    readData= readData >> 4;
    tempInC=readData * 0.0625;


    messageToSend.timeStamp=(float)ticksToSec;
    messageToSend.source = "temp";
    messageToSend.value=tempInC;

    if(tempInC>=23.0) //If the value is greater than threshold, then send the notification to alert task.
    {
        xTaskNotify(g_alertHandle, 1, eSetBits );
    }

    //Sending the data via message queue to the logger task*/
    if(loggerQueue!=0)
    {
        if( xQueueSend( loggerQueue, (void *)&messageToSend, 1000) != pdPASS )
        {
            UARTprintf("Failed to post the message, even after 10 ticks\n");
        }
    }
    /*******************************************************/
}

void tempTask(void *param)
{
    UARTprintf("Entered Temp Task\n");
    //i2cInit();
    tempTimer = xTimerCreate("tempTimer1", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, vtempTimerCallBackFunction);
    if(tempTimer==NULL)
    {
        UARTprintf("Temp Softimer creation FAILED\n");
    }
    else
    {
        UARTprintf("Softimer creation successfull\n");
        if(xTimerStart(tempTimer,100)!=pdPASS)
        {
            UARTprintf("Temp Softimer setting FAILED\n");
        }
    }
    while(1)
    {

    }
}

int tempTaskInit()
{
    if(xTaskCreate(tempTask, (const char *)"tempTask",
                   1024, NULL,1,&g_tempHandle) != pdTRUE)
    {
        // Task creation failed.
        UARTprintf("Task creation failed");
        return(1);
    }

    if(g_tempHandle==NULL)
    {
        UARTprintf("Temp Task handle null");
    }

    return 0;
}

void i2cInit()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    /*Configure alternate functions for the pins*/
    ROM_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    ROM_GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    /*Select the functions for the pin i.e. whether they are SCL or SDA*/
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    /*Set the i2C transfer rate to be 100kbps*/
}

void tempi2cWrite(uint8_t regAdd, uint8_t data)
{
    /*Sending the slave address and register to which the write needs to be done*/

    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVEADD, false);

    I2CMasterDataPut(I2C0_BASE, regAdd);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(!I2CMasterBusy(I2C0_BASE));

    while(I2CMasterBusy(I2C0_BASE));

    /*Now sending the data*/
    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVEADD, false);

    I2CMasterDataPut(I2C0_BASE, data);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(!I2CMasterBusy(I2C0_BASE));

    while(I2CMasterBusy(I2C0_BASE));

}

uint32_t tempi2cRead(uint8_t registerAdd)
{
    uint32_t receivedData;
    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVEADD, false);

    I2CMasterDataPut(I2C0_BASE, registerAdd);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(!I2CMasterBusy(I2C0_BASE));

    while(I2CMasterBusy(I2C0_BASE));

    /*Now receiving the data*/
    I2CMasterSlaveAddrSet(I2C0_BASE, SLAVEADD, true);

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    while(!I2CMasterBusy(I2C0_BASE));

    while(I2CMasterBusy(I2C0_BASE));

    receivedData=I2CMasterDataGet(I2C0_BASE);

    receivedData = receivedData << 8; /*MSB*/

    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    while(!I2CMasterBusy(I2C0_BASE));

    while(I2CMasterBusy(I2C0_BASE));

    receivedData |= I2CMasterDataGet(I2C0_BASE);

    return receivedData;
}





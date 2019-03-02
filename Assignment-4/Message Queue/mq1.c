/*
*@filename 	: mq1.c
*@description 	: file to take care of process 1 functions. Responsible for creating the queue and listening for data from process 2 and then sending new data to process 2.
		  This process is repeated 10 times. 
*@author	: Puneet Bansal
*@reference	: For timestamp: timer file provided in MyCuinfo
*/

#include <fcntl.h> 
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>

#define QUEUENAME "/punsqueue"
#define MAX_SIZE 10

char filename[30];
FILE *fptr_log;

struct timespec thTimeSpec;
static void SIGINT_Handler(int x)
{
fptr_log=fopen(filename,"a");
printf("\nReceived sigint");
clock_gettime(CLOCK_REALTIME, &thTimeSpec);
fprintf(fptr_log,"\n[S: %ld, ns: %ld] SIGINT received\n -> Now Freeing Resources\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);
fclose(fptr_log);
exit(0);
}

typedef struct
{
	char string[30];
	bool led;
}messageQueue;




int main(int argc, char *argv[])
{
 fptr_log=fopen(argv[1],"a");
 signal(SIGINT,SIGINT_Handler);
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"[S: %ld, ns: %ld] Process1 created with PID: %d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,getpid());
 fprintf(fptr_log,"IPC method being used is MESSAGE QUEUE \n");
 char *process1Data[]={"Process 1 Data 1","Process 1 Data 2","Process 1 Data 3","Process 1 Data 4","Process 1 Data 5","Process 1 Data 6","Process 1 Data 7","Process 1 Data 8","Process 1 Data 9","Process 1 Data 10"};

 int retVal,i;
 messageQueue dataToSend,dataToReceive;
 mqd_t mq;

 /*Setting attributes for the message queue*/
 struct mq_attr mq_attr;
 mq_attr.mq_maxmsg = MAX_SIZE;
 mq_attr.mq_msgsize = sizeof(messageQueue);

 /*Opening Message Queue*/
 mq = mq_open(QUEUENAME, O_CREAT | O_RDWR, 0666, &mq_attr); 
 if(mq == -1)
 {
   perror("Queue opening error");
   exit(1);
 }
/*Receiving the data from process 2 and Sending the data to process 2*/
 i=0;
 while(i<10)
 {
   retVal = mq_receive(mq,(char*)&dataToReceive, sizeof(dataToReceive),0); 
   if(retVal==-1)
   {
     perror("Error receving data in process 1");
     exit(1);
   }
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 1] Message Received from PROCESS 2\n {\n Message:%s\n LED:%d\n}  \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToReceive.string,dataToReceive.led);

 strcpy(dataToSend.string,process1Data[i]);
 dataToSend.led=1;
 retVal = mq_send(mq, (char*)&dataToSend, sizeof(dataToSend),0);
 if(retVal==-1)
 {
   perror("Error while sending");
   exit(1);
 } 

 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 1] Message Sent by PROCESS 1\n {\n Message:%s\n LED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToSend.string,dataToSend.led);
 i++;
 } 
 mq_close(mq);
 mq_unlink(QUEUENAME);
 return 0;
 }

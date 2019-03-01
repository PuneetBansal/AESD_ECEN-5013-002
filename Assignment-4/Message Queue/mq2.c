/*
*@filename 	: mq2.c
*@description 	: file to take care of process 2 functions. Responsible for creating the queue and sending the data to process 1 and then receiving the data.
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
#include <time.h>
#include <stdbool.h>

#define QUEUENAME "/punsqueue"
#define MAX_SIZE 10

/*Payload Queue*/
typedef struct
{
	char string[30];
	bool led;
}messageQueue;

struct timespec thTimeSpec;

int main(int argc, char *argv[])
{
 FILE *fptr_log;
 fptr_log=fopen(argv[1],"a");
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"\n\n[S: %ld, ns: %ld] Process2 created with PID: %d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,getpid());
 char *process2Data[]={"Process 2 Data 1","Process 2 Data 2","Process 2 Data 3","Process 2 Data 4","Process 2 Data 5","Process 2 Data 6","Process 2 Data 7","Process 2 Data 8","Process 2 Data 9","Process 2 Data 10"};

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
i=0;

/*Sending the data to process 1 and then receiving the data from process 2*/
while(i<10)
 {
   strcpy(dataToSend.string,process2Data[i]);
   dataToSend.led=0;

   retVal = mq_send(mq,(char*)&dataToSend, sizeof(dataToSend),0);
   if(retVal==-1)
   {
     perror("Error while sending");
     exit(1);
   } 

   clock_gettime(CLOCK_REALTIME, &thTimeSpec);
   fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 2] Message Sent by PROCESS 2\n {\n Message:%s\n LED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToSend.string,dataToSend.led);
   retVal = mq_receive(mq,(char*)&dataToReceive, sizeof(dataToReceive),0); 
   if(retVal==-1)
   {
     perror("Error receving data in process 1");
     exit(1);
   }
   clock_gettime(CLOCK_REALTIME, &thTimeSpec);
   fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 2] Message Received from PROCESS 1\n {\n Message:%s\n LED:%d\n}    \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToReceive.string,dataToReceive.led);
  i++;

 }

 mq_close(mq);
 mq_unlink(QUEUENAME);
 
 return 0;
}

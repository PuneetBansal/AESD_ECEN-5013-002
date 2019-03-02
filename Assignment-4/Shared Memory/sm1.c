/*
*@filename	: sm1.c
*author		: Puneet Bansal
*description	: Enabling shared memory IPC between a 2 processes by sending 10 messages back and forth.
*/
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>   
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>

#define MEMORYNAME "mySharedMem"
#define SEMNAME1 "mySemaphore1"
#define SEMNAME2 "mySemaphore2"

char filename[30];
FILE *fptr_log;
struct timespec thTimeSpec;

static void SIGINT_Handler(int x)
{
printf("\nReceived sigint");
fptr_log=fopen(filename,"a");
clock_gettime(CLOCK_REALTIME, &thTimeSpec);
fprintf(fptr_log,"\n[S: %ld, ns: %ld] SIGINT received\n -> Now Freeing Resources\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);
sem_unlink(SEMNAME1); 
sem_unlink(SEMNAME2);
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
 strcpy(filename,argv[1]);
 signal(SIGINT,SIGINT_Handler);
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"\n\n[S: %ld, ns: %ld] Process1 created with PID: %d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,getpid());
 fprintf(fptr_log,"IPC method being used is SHARED MEMEORY \n");
 char *process1Data[]={"Process 1 Data 1","Process 1 Data 2","Process 1 Data 3","Process 1 Data 4","Process 1 Data 5","Process 1 Data 6","Process 1 Data 7","Process 1 Data 8","Process 1 Data 9","Process 1 Data 10"};

 int retVal,i;
 messageQueue dataToSend,dataToReceive;
 int sm_fileDescrip= shm_open(MEMORYNAME, O_CREAT | O_RDWR, 0666); 
 
 if(sm_fileDescrip < 0 )
	{
		perror("Access error");
		exit(1);
	}

 /*Memory mapping*/

 int smSize= sizeof(dataToReceive);
 void *memMap = mmap(NULL, smSize, PROT_READ | PROT_WRITE, MAP_SHARED , sm_fileDescrip, 0);	//memory mapping
 if(memMap == (void *)-1)
 {
    perror("Mapping error");
    exit(1);
 }

 /*Opening Semaphore*/
 sem_t * sem1 = sem_open(SEMNAME1, O_CREAT, 0666, 0);
 sem_t * sem2 = sem_open(SEMNAME2, O_CREAT, 0666, 0);

 if(sem1 == NULL || sem2 == NULL)
 {
   perror("Semaphore error");
   exit(1);	
 }

while(i<10)
{
 sem_wait(sem1);
 
 char *recPtr = (char*)&dataToReceive;
 memcpy(recPtr,(char*)memMap, smSize);								//copy to shared memory
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 1] Message received from PROCESS 2\n {\n Message:%s\n LED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToReceive.string,dataToReceive.led);

 
 strcpy(dataToSend.string,process1Data[i]);
 dataToSend.led=1;
 char *sendPtr = (char*)&dataToSend;
 memcpy((char*)memMap, sendPtr, smSize); 
 sem_post(sem2);
 
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 1] Message Sent by PROCESS 1\n {\n Message:%s\n LED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToSend.string,dataToSend.led);

i++;
}

 close(sm_fileDescrip);
 sem_unlink(SEMNAME1); 
 sem_unlink(SEMNAME2);
 fclose(fptr_log);
 return 0;
}

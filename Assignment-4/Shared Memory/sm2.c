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
 signal(SIGINT,SIGINT_Handler);
 strcpy(filename,argv[1]);
fprintf(fptr_log,"\n\n[S: %ld, ns: %ld] Process2 created with PID: %d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,getpid());
 fprintf(fptr_log,"IPC method being used is SHARED MEMEORY \n");
 char *process2Data[]={"Process 2 Data 1","Process 2 Data 2","Process 2 Data 3","Process 2 Data 4","Process 2 Data 5","Process 2 Data 6","Process 2 Data 7","Process 2 Data 8","Process 2 Data 9","Process 2 Data 10"};

 int retVal,i;
 messageQueue dataToSend,dataToReceive;
 int sm_fileDescrip= shm_open(MEMORYNAME, O_CREAT | O_RDWR, 0666); 
 int smSize= sizeof(dataToReceive);
 
 if(sm_fileDescrip < 0 )
	{
		perror("Access error");
		exit(1);
	}
 int value = ftruncate(sm_fileDescrip, smSize);
 if(value < 0)
 {
	perror("Ftruncate error");
	exit(1);
 }

 /*Memory mapping*/

 void *memMap = mmap(NULL, smSize, PROT_READ | PROT_WRITE, MAP_SHARED , sm_fileDescrip, 0);	//memory mapping
 if(memMap == (void *)-1)
 {
    perror("Mapping error");
    exit(1);
 }

 /*Opening Semaphore*/
 sem_t * sem1 = sem_open(SEMNAME1, O_CREAT, 0666, 0);
 sem_t * sem2 = sem_open(SEMNAME2, O_CREAT, 0666, 0);

 if(sem1 == NULL || sem2==NULL)
 {
   perror("Semaphore error");
   exit(1);	
 }

while(i<10)
{
 strcpy(dataToSend.string,process2Data[i]);
 dataToSend.led=0;
 char *sendPtr = (char*)&dataToSend;
 memcpy((char*)memMap, sendPtr, smSize); 
 sem_post(sem1);
 
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 2] Message Sent by PROCESS 2\n {\n Message:%s\n LED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToSend.string,dataToSend.led);

 sem_wait(sem2);
 char *recPtr = (char*)&dataToReceive;
 memcpy(recPtr,(char*)memMap, smSize);								//copy to shared memory
 clock_gettime(CLOCK_REALTIME, &thTimeSpec);
 fprintf(fptr_log,"[S: %ld, ns: %ld][PROCESS 2] Message received from PROCESS 1\n {\n Message:%s\n LED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,dataToReceive.string,dataToReceive.led);

 i++;
}
 close(sm_fileDescrip);
 sem_unlink(SEMNAME1);
 sem_unlink(SEMNAME2);
 fclose(fptr_log);
 return 0;
}



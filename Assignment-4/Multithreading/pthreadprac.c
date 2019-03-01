/*
*@filename: pthreaprac.c
*@author  : Puneet Bansal
*@description: Creating 2 child threads inside a parent thread. Child thread one counts the number of alphabets from an input file and dumps it in a logfile if the count of the alphabet is less than 100(A or a both counts). The second child thread prints the cpu utilisation every 100ms. This thread can be killed using the kill command from command line.
*/

#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>


#define BUFFER_SIZE 10

#define USR1_SIGNAL 1<<0
#define USR2_SIGNAL 1<<1
#define TIMER_HANDLER 1<<2
 
volatile int sig;
volatile int usr1,usr2;
pthread_t childThread[2]; 			/*Create 2 child threads in the main thread*/
timer_t timer;
pthread_mutex_t lock;

static timer_t timerid;
struct timespec thTimeSpec;

/*Structure to store the name of the file input from command line*/
typedef struct
{
char * name;
}fileStruct;


static void timerHandler()
{
printf("Entered timer handler\n");
sig|=TIMER_HANDLER;
}

/*
*@desc: removing the p threads created, on receiving the USR1 signal
*/

void USR1_Handler(int x)
{
int y,z;
//timer_delete(timer);
sig|=USR1_SIGNAL;
usr1=1;

printf("Received USR1 signal\n");
//pthread_cancel(childThread[0]);
pthread_cancel(childThread[1]);
}


/*
*@desc: removing the p threads created, on receiving the USR2 signal
*/

void USR2_Handler(int x)
{
int y,z;
timer_delete(timer);
sig|=USR2_SIGNAL;
usr2=1;
printf("Received USR2 signal\n");
//pthread_cancel(childThread[0]);
pthread_cancel(childThread[1]);
}


/*
*@description: Fxn to execute when childThread[0] is running
*@param : arg
*/

static void* threadFxn1(void * argv)
{
	char *inputBuffer, *logBuffer;	
	FILE *fptr_log,*fptr_input;
	long long len;
	int arr[26];
	
	pthread_mutex_lock(&lock);
	fptr_log= fopen(argv,"a");
	
	printf("Entered the first thread with PID: %d and TID: %d\n",getpid(),(pid_t)syscall(SYS_gettid));
	fprintf(fptr_log,"\nEntered the first thread with PID: %d and TID: %d and POSIX thread id: %ld\n",getpid(),(pid_t)syscall(SYS_gettid),pthread_self());
	clock_gettime(CLOCK_REALTIME, &thTimeSpec);
	fprintf(fptr_log,"[THREAD 1]Clock_getttime: s:%ld ns:%ld - \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);
	fclose(fptr_log);
	pthread_mutex_unlock(&lock);

	/*Opening the input file in readonly mode and storing the contents of file in a buffer*/	
	fptr_input=fopen("gdb.txt","r");
	if(fptr_input!=NULL)
	{
		printf("Input file opening successfull\n");		
		
		fseek(fptr_input,0,SEEK_END);
		len=ftell(fptr_input);
		fseek(fptr_input,0,SEEK_SET);
		inputBuffer=malloc(sizeof(char)*len);
		logBuffer=malloc(sizeof(char)*len);
			
		if(inputBuffer!=NULL)
  		{
    			fread(inputBuffer, 1, len, fptr_input);
  		}
		
		printf("Input file read successfully\n");
		
		/*Sorting the contents of the file in alphabetical order*/

		int i,j;
    		char ch_temp;
    		/*for(i=0;i<len-2;i++)
    		{
        		for(j=i+1;j<len-1;j++)
            		{
                		if(inputBuffer[j]<inputBuffer[i])
                		{
                 		ch_temp=inputBuffer[i];
                 		inputBuffer[i]=inputBuffer[j];
                 		inputBuffer[j]=ch_temp;
                		}
            		}
    		}*/

		
	for(i=0;i<len-1;i++)
	{
		if(inputBuffer[i]>=65 && inputBuffer[i]<=90) 
		{
		arr[inputBuffer[i]-65]++;
		}
		else if(inputBuffer[i]>=97 && inputBuffer[i]<=122)
		{
		arr[inputBuffer[i]-97]++;	
		}
	}
   
	pthread_mutex_lock(&lock);
	fptr_log= fopen(argv,"a");
	
    	for(i=0;i<26;i++)
	{
		if(arr[i]<100)
		{
			printf("alphabet is %c and count is %d\n",i+65,arr[i]);	
			fprintf(fptr_log,"alphabet is %c and count is %d\n",i+65,arr[i]);
		}	
	}
	fclose(fptr_log);
	pthread_mutex_unlock(&lock);
		
		
		
	}
	else
	{
		printf("Operation to open input file failed\n");
	}
			
	printf("Exitting thread 1 normally\n");
	pthread_mutex_lock(&lock);
	fptr_log= fopen(argv,"a");
	fprintf(fptr_log,"Exitting thread 1 normally\n");
	fclose(fptr_log);
	pthread_mutex_unlock(&lock);
	fclose(fptr_input);

	return NULL;

}

/*
*@description: Fxn to execute when childThread[1] is running
*@param :arg
*/

static void* threadFxn2(void *argv)
{
	FILE *fptr_log, *fptr_timer;
	fptr_log= fopen(argv,"a");
	char procStat[30];
	int i=0;
	printf("Entered the second thread with PID: %d and TID: %d\n",getpid(),(pid_t)syscall(SYS_gettid));
	fprintf(fptr_log,"\nEntered the second thread with PID: %d and Linux TID: %d and POSIX thread id %ld\n",getpid(),(pid_t)syscall(SYS_gettid),pthread_self());	

	clock_gettime(CLOCK_REALTIME, &thTimeSpec);
	fprintf(fptr_log,"[THREAD 2]TimeStamp: s:%ld ns:%ld- \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);

	struct sigevent sev;	
	struct sigaction sigAct;
	struct itimerspec itimerSpec;
	
	memset(&sigAct,0,sizeof(sigAct));
	sigAct.sa_flags=SA_SIGINFO;
	sigAct.sa_sigaction = timerHandler;		
	sigaction(SIGRTMIN,&sigAct,NULL);		
	
	sev.sigev_notify = SIGEV_SIGNAL;
        sev.sigev_signo = SIGRTMIN;
        //sev.sigev_value.sival_ptr = argv;

        timer_create(CLOCK_REALTIME, &sev, &timer);
	
	/*Setting timer values and interval*/	
	itimerSpec.it_value.tv_sec=0;
	itimerSpec.it_value.tv_nsec=100000000;
	
	itimerSpec.it_interval.tv_sec=0;
	itimerSpec.it_interval.tv_nsec=100000000;
	
	timer_settime(timer, 0, &itimerSpec, NULL);	
	while(1)
	{
		if(sig & TIMER_HANDLER)
		{
			sig &= ~TIMER_HANDLER;
			fptr_timer=popen("cat /proc/stat | grep 'cpu'","r");
			if(fptr_timer!=NULL)
			{
			printf("popen successfully\n");	
			pthread_mutex_lock(&lock);
			
			fgets(procStat,200,fptr_timer);
			fprintf(fptr_log,"\n\nProc Stat Details\n");
			fprintf(fptr_log,"%s\n",procStat);
printf("Here\n");
			
			/*fgets(procStat,200,fptr_timer);
			fprintf(fptr_log,"%s\n",procStat);


			
			fgets(procStat,200,fptr_timer);
			fprintf(fptr_log,"%s\n",procStat);

			fgets(procStat,200,fptr_timer);
			fprintf(fptr_log,"%s\n",procStat);*/

			pclose(fptr_timer);
			pthread_mutex_unlock(&lock);

			}
			else
			{
			printf("popen failed\n");
			} 	
		
		}

	}
	printf("Exitting thread 2 normally\n");
	fprintf(fptr_log,"Exitting thread 2 normally\n");	
	fclose(fptr_log);
	return NULL;
}


int main(int argc, char *argv[])
{
	fileStruct file;
	FILE* fptr_log;
        file.name=malloc(20);
	strcpy(file.name,argv[1]);
	printf("file name is %s\n",argv[1]);
	fptr_log=fopen(argv[1],"a");
	pthread_mutex_init(&lock,NULL);
	printf("Main thread created with PID: %d and TID: %d\n",getpid(),(pid_t)syscall(SYS_gettid));
	fprintf(fptr_log,"\n\nMain thread created with PID: %d and TID: %d and POSIX thread id : %ld\n",getpid(),(pid_t)syscall(SYS_gettid),pthread_self());
	clock_gettime(CLOCK_REALTIME, &thTimeSpec);
	fprintf(fptr_log,"[MAIN]Timestamp: s:%ld ns:%ld - \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);
	
	
	signal(SIGUSR1,USR1_Handler);
	signal(SIGUSR2,USR2_Handler);
		
	pthread_create(&childThread[0],NULL,threadFxn1,(void*)file.name);	
	pthread_create(&childThread[1],NULL,threadFxn2,(void*)file.name);	
	
	pthread_join(childThread[0],NULL);		/*Main thread waits until childThead[0] completes*/		
	pthread_join(childThread[1],NULL);		/*Main thread waits until childThead[1] completes*/
	timer_delete(timer);
	printf("after pthread join\n");
	printf("sig is %d",sig);
	if(usr1)
		{
		printf("Exited due to USR1 signal\n");
		fprintf(fptr_log,"Exited due to USR1 signal\n");
		clock_gettime(CLOCK_REALTIME, &thTimeSpec);
		fprintf(fptr_log,"[USR1]Clock_getttime: %ld ns:%ld- \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);
		
		sig &= ~USR1_SIGNAL;
		usr1=0;
		}
	if(usr2)
		{
		fprintf(fptr_log,"Exited due to USR2 signal\n");
	
		clock_gettime(CLOCK_REALTIME, &thTimeSpec);
	        fprintf(fptr_log,"[USR2] Clock_getttime: %ld ns:%ld- \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);
		
		sig &= ~USR2_SIGNAL;
		usr2=0;
		}
	fprintf(fptr_log,"Exitting main thread\n");
	clock_gettime(CLOCK_REALTIME, &thTimeSpec);
	fprintf(fptr_log,"[MAIN EXIT] Timestamp: %ld ns:%ld- \n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec);
	fclose(fptr_log);
	return 0;
}

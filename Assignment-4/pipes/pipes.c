/*
*@filename	: pipes.c
*author		: Puneet Bansal
*description	: Enabling pipe based IPC between a parent and forked process by sending 10 messages.
*@reference 	: https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/

*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>

typedef struct 
{
char string[20];
bool led;
}pipetest;

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

int main(int argc, char *argv[])
{
	int i;	
	char *strarrParent[]={"Parent String 1","Parent String 2","Parent String 3","Parent String 4","Parent String 5","Parent String 6","Parent String 7","Parent String 8","Parent 		String 9","Parent String 10"};
	strcpy(filename,argv[1]);	
	signal(SIGINT,SIGINT_Handler);
	fptr_log=fopen(argv[1],"a");
	clock_gettime(CLOCK_REALTIME, &thTimeSpec);
	fprintf(fptr_log,"[S: %ld, ns: %ld] Main thread created with PID: %d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,getpid());
	fprintf(fptr_log,"IPC method being used is PIPES \n");
	
	int pipe1[2]; /*To create reading and writing ends of pipe1*/	
	int pipe2[2]; /*To create reading and writing ends of pipe2*/
	pid_t process;

	if (pipe(pipe1)==-1) 
	{	 
        	fprintf(fptr_log,"Pipe Failed" ); 
        	
		return 1; 
    	} 
    	if (pipe(pipe2)==-1) 
    	{ 
        	fprintf(fptr_log,"Pipe Failed" ); 
        	return 1; 
    	}

	process=fork();
	if(process<0)
	{
		fprintf(fptr_log,"Fork failed");
		return 1;
	}

	else if (process > 0) 
    	{ 
        	pipetest sendObj,recObj;
		close(pipe1[0]); /*Closing reading end of pipe1*/
		close(pipe2[1]); // Close writing end of second pipe 
        	
		/*Sending one message to child and receving a message from child. This process is repeated 10 times*/
		for(i=0;i<10;i++)
		{
		strcpy(sendObj.string,strarrParent[i]);
		sendObj.led=1;
		write(pipe1[1], &sendObj, sizeof(sendObj)); 
        	//wait(NULL);
		read(pipe2[0], &recObj, 100); 
		clock_gettime(CLOCK_REALTIME, &thTimeSpec);
        	fprintf(fptr_log,"[S: %ld, ns: %ld][Parent process] Values read from child: \n string -> %s\n boolean value ->%d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,recObj.string,recObj.led);
		}
	        
		close(pipe1[1]);
		close(pipe2[0]); 
    } 

else
    { 
        char *strarrChild[]={"Child String 1","Child String 2","Child String 3","Child String 4","Child String 5","Child String 6","Child String 7","Child String 8","Child String 		9","Child String 10"};
	close(pipe1[1]);  // Close writing end of first pipe 
	close(pipe2[0]);
  	pipetest ro,so;

	/*Reading the message from parent and sending a message to parent. This process is repeateed 10 times.*/
	for(i=0;i<10;i++)
	{	
	strcpy(so.string,strarrChild[i]);
	so.led=0;
        read(pipe1[0], &ro, sizeof(ro)); // Read a structure using first pipe 
	clock_gettime(CLOCK_REALTIME, &thTimeSpec);
  	fprintf(fptr_log,"[S: %ld, ns: %ld][Child process] Value read from parent :\n string->%s \n boolean value ->%d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,ro.string,ro.led);
      	
 	write(pipe2[1], &so, sizeof(so));// Write the structure using second pipe.
        }
	
	close(pipe1[0]);
	close(pipe2[1]); 
  
        exit(0); 
    } 
} 
	





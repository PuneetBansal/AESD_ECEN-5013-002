/*
*@filename 	: server.c
*@description 	: file handling the server side operations of socket IPC.
*@author	: Puneet Bansal
*@reference	: https://www.geeksforgeeks.org/socket-programming-cc/ 
                  For timestamp: timer file provided in MyCuinfo
*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <pthread.h>
#include <time.h>


#define PORT_ADR 8000
struct timespec thTimeSpec;

typedef struct
{
  char    string[20];
  bool    led;
}socketTest;


int main(int argc, char *argv[])
{
  char *arrServer[]={"Message 1 from Server","Message 2 from Server","Message 3 from Server","Message 4 from Server","Message 5 from Server","Message 6 from Server","Message 7 from Server","Message 8 from Server","Message 9 from Server","Message 10 from Server"};

  FILE *fptr_log;
  fptr_log=fopen(argv[1],"a");
  clock_gettime(CLOCK_REALTIME, &thTimeSpec);
  fprintf(fptr_log,"[S: %ld, ns: %ld] Served created with PID: %d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,getpid());
  fprintf(fptr_log,"IPC method being used is SOCKETS \n");
  
  int send_return;

  socketTest payload;
  socketTest payload2;
  socketTest* payloadptr;
  size_t payloadLen = 0;

  struct sockaddr_in addr, peer_addr;
  int addr_len = sizeof(peer_addr);

  char rdbuff[1024] = {0};
  int server_socket, accepted_soc, opt = 1;
  int i = 0;
  
  int read_b;
 
  /* create socket */
 
 if((server_socket = socket(AF_INET,SOCK_STREAM,0)) == 0)
  {
    fprintf(fptr_log,"[Server] [ERROR] Socket Creation Error\n");
    return 1;
  }
  else
    fprintf(fptr_log,"[Server] Socket Created Successfully\n");

  /* set socket options */
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(opt), sizeof(opt)))
  {
    fprintf(fptr_log,"[Server] [ERROR] Socket options set error\n");
    return 1;
  }

  /*Set the sockaddr_in structure */
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;	
  addr.sin_port = htons(PORT_ADR);

  /*bind socket to a address */
  if((bind(server_socket,(struct sockaddr*)&addr, sizeof(addr))) < 0)
  {
    fprintf(fptr_log,"[Server] [ERROR] Bind socket Error\n");
    return 1;
  }
  else
    fprintf(fptr_log,"[Server] Socket binded Successfully\n");

  /* listen for connections*/
  if(listen(server_socket,5) < 0)
  {
    fprintf(fptr_log,"[Server] [ERROR] Can't listen connection\n");
    return 1;
  }

  /*accept connection */
  accepted_soc = accept(server_socket, (struct sockaddr*)&peer_addr,(socklen_t*)&addr_len);
  if(accepted_soc < 0)
  {
    fprintf(fptr_log,"[Server] [ERROR] Can't accept connection\n");
    return 1;
  }
int j=0;
while(j<10)
{
  /* read payload length */
  read_b = read(accepted_soc, &payloadLen, sizeof(size_t));
  if(read_b == sizeof(size_t))
  {
    clock_gettime(CLOCK_REALTIME, &thTimeSpec);
    fprintf(fptr_log,"[S: %ld, ns: %ld][Server] Size of incoming payload: %ld\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,payloadLen);
  }	
  else
  {
    fprintf(fptr_log,"[Server] [ERROR] Invalid data\n");
    return 1;
  } 

  /* read payload */
  while((read_b = read(accepted_soc, rdbuff+i, 1024)) < payloadLen)
  {
    i+=read_b;	
  }
  payloadptr= (socketTest*)rdbuff;
  /* display data */
  clock_gettime(CLOCK_REALTIME, &thTimeSpec);
  fprintf(fptr_log,"[S: %ld, ns: %ld][Server] Message Received from Client\n {\n Message:%s\n USRLED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,payloadptr->string, payloadptr->led);
  j++;
  }

  /* send message from server to client 
  send(accepted_soc , "ACK" , 4, 0);
  printf("[Server] Message sent from Server: ACK\n");*/

int k=0;
 /* Populating payload structure */

 //const char msg[20];
 

 while(k<10)
 {
 strcpy(payload2.string,arrServer[k]);
 payload2.led = 0;
  /*Sending the size of the payload*/
  payloadLen = sizeof(payload2); 
  send_return=send(accepted_soc , &payloadLen, sizeof(size_t), 0 );
  printf("send: %d \n", send_return);
  /*Sending the payload */
  send_return = send(accepted_soc , (char*)&payload2 , sizeof(payload2), 0 );
  /* check whether all the bytes are sent or not */
  
  if(send_return < sizeof(payload2))
  {
    fprintf(fptr_log,"[Server] [ERROR] Complete data not sent\n");
    return 1;
  }
  
  /* display the date sent */
  clock_gettime(CLOCK_REALTIME, &thTimeSpec);
  fprintf(fptr_log,"[S: %ld, ns: %ld][Server] Message sent from Server\n{\n Message: %s\n LED: %d\n}\n", \
                           thTimeSpec.tv_sec,thTimeSpec.tv_nsec,payload2.string, payload2.led);
  sleep(1);
 k++;
 }
  
  fclose(fptr_log);
  /*close socket */

  close(accepted_soc);

  return 0;
}

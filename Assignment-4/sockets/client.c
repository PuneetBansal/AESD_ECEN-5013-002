/*
*@filename 	: client.c
*@description 	: file handling the client side operations of socket IPC.
*@author	: Puneet Bansal
*@reference	: https://www.geeksforgeeks.org/socket-programming-cc/ 
                  For timestamp: timer file provided in MyCuinfo
*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <time.h>


#define PORT_ADR    8000
#define IP_ADR      "127.0.0.1" /* IP Address*/

typedef struct
{
  char    string[20];
  bool    led;
}socketTest;

struct timespec thTimeSpec;
int main(int argc, char *argv[])
{
  FILE *fptr_log;
  fptr_log=fopen(argv[1],"a");
  char *arrClient[]={"Message 1 from Client","Message 2 from Client","Message 3 from Client","Message 4 from Client","Message 5 from Client","Message 6 from Client","Message 7 from Client","Message 8 from Client","Message 9 from Client","Message 10 from Client"}	;
  clock_gettime(CLOCK_REALTIME, &thTimeSpec);
  
  fprintf(fptr_log,"[S: %ld, ns: %ld] Client file created with PID: %d\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,getpid());
  fprintf(fptr_log,"IPC method being used is SOCKET \n");
   
  size_t read_b;
  int client_socket = 0;
  char rdbuff[1024] = {0};
  int accepted_soc, opt = 1;
  int i = 0;
  struct sockaddr_in servAddr = {0};
 
  int send_return;
  size_t payloadLen=0;
  socketTest payload,payloadRead;
  socketTest *payloadptr;
  char r_data[4] = {0};

  /* create socket */
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    fprintf(fptr_log,"[Client] [ERROR] Socket creation Error\n");
    return -1;
  }
  else
    fprintf(fptr_log,"[Client] Socket Created Successfully\n");

  /* Fill the socket address structure */
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT_ADR);
      
  /* convert the IP ADDR to proper format */
  if(inet_pton(AF_INET, IP_ADR, &servAddr.sin_addr)<=0) 
  {
    fprintf(fptr_log,"[Client] [ERROR] Address Conversion Error\n");
    return -1;
  }
  
  /* connect the socket before sending the data */
  if (connect(client_socket, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
  {
    fprintf(fptr_log,"[Client] [ERROR] Connection Failed \n");
    return -1;
  }
	int j=0;
  
 while(j<10)
 {
  strcpy(payload.string,arrClient[j]);
  payload.led = 1;

  /*Sending the size of the payload*/
  payloadLen = sizeof(payload); 
  send_return=send(client_socket , &payloadLen, sizeof(size_t), 0 );

  /*Sending the payload */
  send_return = send(client_socket , (char*)&payload , sizeof(payload), 0 );
  /* check whether all the bytes are sent or not */
  if(send_return < sizeof(payload))
  {
    fprintf(fptr_log,"[Client] [ERROR] Complete data not sent\n");
    return 1;
  }
  
  /* display the date sent */
  fprintf(fptr_log,"[Client] Message sent from Client\n{\n Message: %s\n LED: %d\n}\n", \
                           payload.string, payload.led);
  sleep(1);
 j++;
 }

  /* read data sent by server 
  read(client_socket, r_data, 4);
  printf("[Client]  Message received from Server: %s\n",r_data);*/

int k=0;
while(k<10)
{
  /* read payload length */
  read_b = read(client_socket, &payloadLen, sizeof(size_t));

  
 
  if(read_b == sizeof(size_t))
  {
    clock_gettime(CLOCK_REALTIME, &thTimeSpec);
    fprintf(fptr_log,"[S: %ld, ns: %ld][Client] Size of incoming payload: %ld\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,payloadLen);
  }	
  else
  {
    fprintf(fptr_log,"[Client] [ERROR] Invalid data\n");
    return 1;
  } 

  /* read payload */
  while((read_b = read(client_socket, rdbuff+i, 1024)) < payloadLen)
  {
    i+=read_b;	
  }
  payloadptr= (socketTest*)rdbuff;
  /* display data */

  clock_gettime(CLOCK_REALTIME, &thTimeSpec);
  fprintf(fptr_log,"[S: %ld, ns: %ld][Client] Message Received from Server\n {\n Message:%s\n USRLED:%d\n}\n",thTimeSpec.tv_sec,thTimeSpec.tv_nsec,payloadptr->string, payloadptr->led);
  k++;
}


  fclose(fptr_log);
  /* close socket */ 
  close(client_socket);
  
  return 0;
}

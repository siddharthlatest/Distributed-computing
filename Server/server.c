/**
 *server.c 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MAX_CLIENTS 50				// Upper limit on the number of clients connected at the same time
#define DATA_SIZE 1024				// Upper limit on the size of data to be sent/recieved

int main() {

  const int true = 1;				// Using "true" to enhance readability
  int cntr_connected = -1; 			// Maintains the number of connected clients at the moment
  int sock_desc;				// Server's Socket descriptor
  int connected[MAX_CLIENTS] = {0};		// Client's socket descriptor array
  int bytes_recieved;  				// Size of the recieved data (<= DATA_SIZE)
  char send_data[DATA_SIZE];			// Message to be sent
  char recv_data[DATA_SIZE];       		// Message to be recieved
  struct sockaddr_in server_addr;		// Server socket information
  struct sockaddr_in client_addr;    		// Client socket information
  int sin_size;					// sizeof struct sockaddr_in

  // Specify the type of communication protocol      
  if ((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket");
    exit(1);
  }

  if (setsockopt(sock_desc,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
    perror("Setsockopt");
    exit(1);
  }
        
  server_addr.sin_family = AF_INET;         
  server_addr.sin_port = htons(5000);     
  server_addr.sin_addr.s_addr = INADDR_ANY; 
  bzero(&(server_addr.sin_zero),8); 

  if (bind(sock_desc, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
    perror("Unable to bind");
    exit(1);
  }

  if (listen(sock_desc, 5) == -1) {
    perror("Listen");
    exit(1);
  }
		
  printf("\nTCPServer Waiting for client on port 5000");
  fflush(stdout);

  while(true) {  

    sin_size = sizeof(struct sockaddr_in);
    connected[++cntr_connected] = accept(sock_desc, (struct sockaddr *)&client_addr, &sin_size);

    if (fork() == 0) {
      printf("\n I got a connection from (%s , %d)",
          inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

      while (true) {
        printf("\n SEND (q or Q to quit) : ");
        scanf("%[^\n]", send_data);
        getchar();
        fflush(stdout);      
        if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0) {
          send(connected[cntr_connected], send_data,strlen(send_data), 0); 
          close(connected[cntr_connected]);
          break;
        } else
          send(connected[cntr_connected], send_data,strlen(send_data), 0);  

        bytes_recieved = recv(connected[cntr_connected], recv_data, DATA_SIZE, 0);
        recv_data[bytes_recieved] = '\0';

        if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
          close(connected[cntr_connected]);
	  cntr_connected--;
          break;
        } else 
          printf("\n RECIEVED DATA = %s " , recv_data);
          fflush(stdout);
      }
    } // End of child process
    close(connected[cntr_connected]);	//Parent closes connected socket
  }       
  
  close(sock_desc);
  return 0;
} 

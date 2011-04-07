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
#include <pthread.h>
#include "data_recieve.c"

#define MAX_CLIENTS 50				// Upper limit on the number of clients connected at the same time
#define DATA_SIZE 1024				// Upper limit on the size of data to be sent/recieved


int cntr_connected = -1; 			// Maintains the number of connected clients at the moment

int main() {

  const int true = 1;				// Using "true" to enhance readability
  int sock_desc;				// Server's Socket descriptor
  int connected[MAX_CLIENTS] = {0};		// Client's socket descriptor array
  int bytes_recieved;  				// Size of the recieved data (<= DATA_SIZE)
  char recv_data[DATA_SIZE];       		// Message to be recieved
  char send_data[DATA_SIZE];			// Message to be sent
  struct sockaddr_in server_addr;		// Server socket information
  struct sockaddr_in client_addr;    		// Client socket information
  int sin_size;					// sizeof struct sockaddr_in
  pthread_t tid[MAX_CLIENTS];			// Thread Id's for each client communication

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
    printf("Number of clients now: %d\n", cntr_connected+1);
    pthread_create(&tid[cntr_connected], NULL, data_recieve, (void *)connected[cntr_connected]);
  }       
  
  pthread_join(tid[cntr_connected], NULL);
  int i = 0;
  for (i = 0; i < cntr_connected; i++)
    close(connected[cntr_connected]);
  close(sock_desc);
  return 0;
} 

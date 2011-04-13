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
#include "data_communication.c"

#define MAX_CLIENTS 50				// Upper limit on the number of clients connected at the same time
#define DATA_SIZE 1024				// Upper limit on the size of data to be sent/recieved


int cntr_connected = 0; 			// Maintains the number of connected clients at the moment
int client_to_send = -1;
int connected[MAX_CLIENTS] = {0};		// Client's socket descriptor array
int server_exit = 0;

int main() {

  const int true = 1;				// Using "true" to enhance readability
  int sock_desc;				// Server's Socket descriptor
  int bytes_recieved;  				// Size of the recieved data (<= DATA_SIZE)
  char recv_data[DATA_SIZE];       		// Message to be recieved
  char send_data[DATA_SIZE];			// Message to be sent
  struct sockaddr_in server_addr;		// Server socket information
  struct sockaddr_in client_addr;    		// Client socket information
  int sin_size;					// sizeof struct sockaddr_in
  pthread_t tid[MAX_CLIENTS+5];			// Thread Id's for each client communication
  pthread_t tid_send;				// Thread Id for the server send

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
		
  printf("TCPServer Waiting for client on port 5000 ...\n");
  fflush(stdout);

  pthread_create(&tid_send, NULL, data_send, (void *)NULL); 
  // Thread for sending data to a client

  while(true) {  
    if (server_exit == 1) // Check if the server wants to exit
      break;
    sin_size = sizeof(struct sockaddr_in);
    connected[cntr_connected] = accept(sock_desc, (struct sockaddr *`)&client_addr, &sin_size);
    printf("--- Number of clients now: %d\n", cntr_connected+1);
    client_to_send = cntr_connected;
    pthread_create(&tid[cntr_connected], NULL, data_recieve, (void *)connected[cntr_connected]);
    cntr_connected++;
  }       
  
  int i = 0;
  for (i = 0; i < cntr_connected; i++)
    close(connected[cntr_connected]);
  close(sock_desc);
  return 0;
} 

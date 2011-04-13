/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "data_communication.c"

#define DATA_SIZE 1024
#define BIND_ADDR 5000

int main() {
  int sock, bytes_recieved;  
  char send_data[DATA_SIZE],recv_data[DATA_SIZE];
  struct hostent *host;
  struct sockaddr_in server_addr;  
  pthread_t send, receive;
  host = gethostbyname("127.0.0.1");

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket");
    exit(1);
  }

  server_addr.sin_family = AF_INET;     
  server_addr.sin_port = htons(BIND_ADDR);   
  server_addr.sin_addr = *((struct in_addr *)host->h_addr);
  bzero(&(server_addr.sin_zero),8); 

  if (connect(sock, (struct sockaddr *)&server_addr, 
                      sizeof(struct sockaddr)) == -1) {
    perror("Connect");
    exit(1);
  }

  pthread_create(&send, NULL, data_send, (void *)sock);
  pthread_create(&receive, NULL, data_receive, (void *)sock);
  pthread_join(send, NULL);
  pthread_join(receive, NULL);
  return 0;
}

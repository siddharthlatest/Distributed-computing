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

int main(int argc, char *argv[]) {

  int sock, bytes_recieved;  
  char send_data[DATA_SIZE],recv_data[DATA_SIZE];
  struct hostent *host;
  struct sockaddr_in server_addr;  
  pthread_t communicate;
  if (argc == 1)   
   host = gethostbyname("127.0.0.1");
  else if (argc == 2) {
   printf("%s\n", argv[1]);
   host = gethostbyname(argv[1]);
  }
  else {
    printf("Usage: <name_of_program> [server_ip]\n");
    return 1;
  }

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

  pthread_create(&communicate, NULL, data_communicate, (void *)sock);
  pthread_join(communicate, NULL);
  return 0;
}

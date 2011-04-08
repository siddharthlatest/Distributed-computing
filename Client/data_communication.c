#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

#define DATA_SIZE 1024

extern int cntr_connected;

void  *data_receive(void *sock) {
  const int true = 1;
  int bytes_recieved;
  char recv_data[DATA_SIZE];

  while (true) {
    bytes_recieved=recv((int)sock,recv_data,1024,0);
    recv_data[bytes_recieved] = '\0';
 
    if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
      close((int)sock);
      break;
    } 
    else
      printf("\nRecieved data = %s " , recv_data);
  }
}

void *data_send(void *sock) {
  char send_data[DATA_SIZE];
  const int true = 1;

  while (true) {
    printf("\nSEND (q or Q to quit) : ");
    scanf("%[^\n]", send_data);
    getchar();
    fflush(stdout);

    send((int)sock,send_data,strlen(send_data), 0);
    if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0) {
      close((int)sock);
      break;
    }    
  }
}

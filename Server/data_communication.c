#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

#define DATA_SIZE 1024

extern int cntr_connected;

void  *data_recieve(void *connector) {
  const int true = 1;
  int bytes_recieved;
  char recv_data[DATA_SIZE];

  while (true) {
    if ((bytes_recieved = recv((int)connector, recv_data, DATA_SIZE, 0)) == -1) {
      perror("recv_call");
    }
    recv_data[bytes_recieved] = '\0';

    if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
      printf ("A client left.\n");
      printf("Send data (q or Q to quit): \n  ");
      fflush(stdout);
      close((int)connector);
      cntr_connected--;
      break;
    } 
    else if (recv_data[0] != '\0') {
      printf("RECIEVED DATA = %s \n" , recv_data);
      printf("Send data (q or Q to quit): \n  ");
    }
    fflush(stdout);
    recv_data[0] = '\0';
  }
}

void *data_send(void *connector) {
  char send_data[DATA_SIZE];
  const int true = 1;
  while (true) {
    printf("Send data (q or Q to quit): \n  ");
    fflush(stdout);
    scanf("%[^\n]", send_data);
    getchar();
    send((int)connector, send_data, strlen(send_data), 0);
    if (strcmp(send_data, "q") == 0 || strcmp(send_data, "Q") == 0) {
      close((int)connector);
    }
    send_data[0] = '\0';
  }
}

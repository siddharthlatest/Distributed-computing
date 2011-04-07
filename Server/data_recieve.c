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
      printf ("A client left.");
      close((int)connector);
      cntr_connected--;
      break;
    } else
       printf("RECIEVED DATA = %s \n" , recv_data);
    fflush(stdout);
  }
}

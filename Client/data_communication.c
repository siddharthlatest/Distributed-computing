#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include "compute.c"

#define DATA_SIZE 1024

extern int cntr_connected;

void  *data_communicate(void *sock) {
  const int true = 1;
  int bytes_recieved;
  char recv_data[DATA_SIZE];

  while (true) {
    bytes_recieved=recv((int)sock, recv_data, DATA_SIZE, 0);
    recv_data[bytes_recieved] = '\0';
 	
    if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
      close((int)sock);
      break;
    } 
    else if (recv_data[0] != '\0') {
      // Scan the input from the recieved data
      int start, end;
      sscanf(recv_data, "%d %d", &start, &end);

      // Perform the computation
      char value[20] = {'\0'};
      compute(start, end, value);
      printf("Computed range: %d %d\n", start, end);

      // Send the data
      send((int)sock, value, strlen(value), 0);    
      printf("SENT data: %s\n", value);
    }
    recv_data[0] = '\0';
    fflush(stdout);
  }
}

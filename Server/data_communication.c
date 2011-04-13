#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>

#define DATA_SIZE 1024
#define MIN_CLIENTS 3
#define SUBPARTS 10

extern int cntr_connected;
extern int client_to_send;
extern int connected[50];
extern int server_exit;
pthread_mutex_t msg_mutex = PTHREAD_MUTEX_INITIALIZER;

int recieved_cntr;

void  *data_recieve(void *connector) {
  const int true = 1;
  int bytes_recieved;
  char recv_data[DATA_SIZE];

  while (server_exit == 0) {
    if ((bytes_recieved = recv((int)connector, recv_data, DATA_SIZE, 0)) == -1) {
      perror("recv_call");
    }
    recv_data[bytes_recieved] = '\0';

    if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
      printf ("A client left.\n");
      fflush(stdout);
      close((int)connector);
      break;
    } 
    else if (recv_data[0] != '\0') {
      recieved_cntr++;
      printf("RECIEVED DATA = %s \n" , recv_data);
      if (recieved_cntr >= client_to_send) 
        pthread_mutex_unlock(&msg_mutex);   // Unlock once answer is recieved from all
    }
    fflush(stdout);
    recv_data[0] = '\0';
  }
}

void *data_send(void *connector) {
  char send_data[DATA_SIZE];
  const int true = 1;
  while (server_exit == 0) {
    if (client_to_send != -1)
      connector = (void *)connected[client_to_send];  
      // Sets the client with whom communication must happen 

    if (connector != NULL && client_to_send >= MIN_CLIENTS-1) {
      int start, end;
      printf("Enter the start and end range: \n  ");
      fflush(stdout);
      scanf("%d %d", &start, &end);

      int i = 0;
      for (; i < floor(SUBPARTS/(client_to_send+1)); i++) {    
        pthread_mutex_lock(&msg_mutex);
        printf("Locked execution\n");
        int j = 0;
        recieved_cntr = 0;  			// Set the recieved cntr to 0
        for (; j <= client_to_send; j++) {

          connector = (void *)connected[j];
          int CONST = (end+1-start)/SUBPARTS;
          printf("CONST = %d, i = %d\n", CONST, i);
          int local_start = i*(client_to_send+1)*CONST + j*CONST;
          int local_end = local_start+CONST;
          sprintf(send_data, "%d %d", local_start, local_end);
          printf("Sending data %s from %d to %d to client %d\n", send_data, local_start, local_end, j+1);
          if (strcmp(send_data, "q") == 0 || strcmp(send_data, "Q") == 0) {
            close((int)connector);
            server_exit = 1;
          }
          send((int)connector, send_data, strlen(send_data), 0);
          send_data[0] = '\0';
        } // Dividing among the clients, j
      } // Number of iterations, i 
    }
  } 
}

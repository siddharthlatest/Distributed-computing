#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BASE 10

void compute(int start, int end, char *c_sum) {
   int sum = 0;
   while (start++ < end) {
     sum++;
     sleep(1);
   }
   sprintf(c_sum, "%d", sum);
}

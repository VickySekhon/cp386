#include <stdio.h>

int collatz_sequence(int); 

int main() {
    printf("Hello, world!\n");
    int x = collatz_sequence(35);
    return 0;
}

int collatz_sequence(int n) {
     printf("%d, ", n);
     while (n != 1) {
          // even 
          if (n % 2 == 0) {
               n = n/2; 
          // odd
          } else {
               n = 3*n + 1; 
          }

          if (n == 1) {
               printf("%d", n);
          } else {
               printf("%d, ", n);
          }

     } 
     return n;
}



#include <stdio.h>
#include <stdlib.h>

int decimalToBinary(int num) {
  int *binary = malloc(15);
  // assuming 14-bit integer
  int loop = 0;
  binary[0] = 1;
  binary[1] = 1;
  binary[2] = 1;
  binary[3] = 1;
  binary[4] = 1;
  binary[5] = 1;
  binary[6] = 1;
  binary[7] = 1;
  binary[8] = 1;
  binary[9] = 1;
  binary[10] = 1;
  binary[11] = 1;
  binary[12] = 1;
  binary[13] = 1;
  // for (int i = 14; i >= 0; i--) {
  //   // calculate bitmask to check whether
  //   // ith bit of num is set or not
  //   int mask = (1 << i);
  //
  //   // ith bit of num is set
  //   if (num & mask) {
  //     binary[loop] = 1;
  //   }
  //   // ith bit of num is not set
  //   else {
  //     binary[loop] = 0;
  //   }
  //   loop++;
  // }
  // return binary;
  for (int i = 0; i < 13; i++) {
    printf("%d", binary[i]);
  }
  return 0;
}

int main(int argc, char **argv) {
  long numero = strtol(argv[1], NULL, 10);
  decimalToBinary(numero);
  // for (int i = 0; i < 14; i++) {
  //   printf("%d", teste[i]);
  // }
  // free(teste);
  return 0;
}

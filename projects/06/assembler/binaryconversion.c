
// #include <stdio.h>
// #include <stdlib.h>

// int *decimalToBinary(int num) {
//   int *binary = malloc(sizeof(int) * 15);
//   // assuming 14-bit integer
//   int loop = 0;
//   for (int i = 14; i >= 0; i--) {
//     // calculate bitmask to check whether
//     // ith bit of num is set or not
//     int mask = (1 << i);

//     // ith bit of num is set
//     if (num & mask) {
//       binary[loop] = 1;
//     }
//     // ith bit of num is not set
//     else {
//       binary[loop] = 0;
//     }
//     loop++;
//   }
//   return binary;
// }

// int main(int argc, char **argv) {
//   long numero = strtol(argv[1], NULL, 10);
//   int *teste = decimalToBinary(numero);
//   for (int i = 0; i < 15; i++) {
//     printf("%d", teste[i]);
//   }
//   printf("\n");
//   free(teste);
//   return 0;
// }

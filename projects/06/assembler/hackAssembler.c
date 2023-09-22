#include <stdio.h>
#include <stdlib.h>

void trim(char *s);
void decimalToBinary(int num);

int main(int argc, char **argv) {
  FILE *fptr;
  fptr = fopen(argv[1], "r");
  if (fptr == NULL) {
    perror("Unable to open file");
    exit(1);
  }
  char line[100];
  while (fgets(line, sizeof(line), fptr)) {
    if (line[0] == '\r' || line[0] == '/') {
      continue;
    }
    trim(line);
    for (int i = 0; line[i] != '\0'; i++) {
      if (line[i] == '/') {
        printf("%s", "\n");
        break;
      }
      // if (!isspace(line[i])) {
      printf("%c", line[i]);
      // }
    }
  }
  return 0;
}

void trim(char *s) {
  // count the number of leading whitespace characters
  int count = 0;
  while (s[count] == ' ')
    count++;

  // if no whitespace characters were found, we don't need to do anything!
  if (count != 0) {
    // shift the remainder of the string contents up by the number of leading
    // white space characters found
    int i = 0;
    while (s[i + count] != '\0') {
      s[i] = s[i + count];
      i++;
    }

    // terminate the string earlier given that the contents are being reduced,
    // the null terminator is shifted up by the number of leading whitespace
    // characters as well
    s[i] = '\0';
  }
}

void decimalToBinary(int num) {
  int binary[15];
  // assuming 14-bit integer
  int loop = 0;
  for (int i = 14; i >= 0; i--) {
    // calculate bitmask to check whether
    // ith bit of num is set or not
    int mask = (1 << i);

    // ith bit of num is set
    if (num & mask) {
      binary[loop] = 1;
    }
    // ith bit of num is not set
    else {
      binary[loop] = 0;
    }
    loop++;
  }
  for (int i = 0; i < 15; i++) {
    printf("%d", binary[i]);
    // printf("%d", i);
  }
}

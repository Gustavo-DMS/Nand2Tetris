#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *s);
int *decimalToBinary(int num);
int isNumber(char *number);
char *separarDest(char *linha);
char *separarComp(char *linha);
char *separarJmp(char *linha);
int *traduzirDest(char *dest);
int *traduzirComp(char *comp);
int *traduzirJmp(char *jmp);

int main(int argc, char **argv) {
  // abre o arquivo passado
  FILE *program;
  FILE *output;
  if (argc < 2) {
    perror("File required");
    exit(1);
  }
  program = fopen(argv[1], "r");
  if (program == NULL) {
    perror("Unable to open file");
    exit(1);
  }
  if (argc < 3) {
    char *outputText = strdup(argv[1]);
    char *asmLocation = strstr(outputText, ".asm");
    if (asmLocation != NULL) {
      asmLocation[0] = '\0';
    }
    outputText = realloc(outputText, strlen(outputText) + 5);
    outputText = strcat(outputText, ".hack");
    output = fopen(outputText, "w");
  } else {
    output = fopen(argv[2], "w");
  }

  if (output == NULL) {
    perror("Unable to create file");
    exit(1);
  }
  char line[100];
  char *symbolKeys[1000] = {"R0",  "R1",  "R2",  "R3",   "R4",     "R5",
                            "R6",  "R7",  "R8",  "R9",   "R10",    "R11",
                            "R12", "R13", "R14", "R15",  "SCREEN", "KBD",
                            "SP",  "LCL", "ARG", "THIS", "THAT"};
  int symbolValue[1000] = {0,  1,  2,  3,  4,     5,     6, 7, 8, 9, 10, 11,
                           12, 13, 14, 15, 16384, 24576, 0, 1, 2, 3, 4};
  int curretSymbolIndex = 23;
  int lineCount = 0;
  int variableValue = 16;

  while (fgets(line, sizeof(line), program)) {
    // verifica se é um comentário delimitado por / ou uma linha vazia
    if (line[0] == '\r' || line[0] == '/') {
      continue;
    }
    if (line[0] == '(') {
      char *symbol = malloc(50);
      int counter = 0;
      for (int i = 1; line[i] != ')'; i++) {
        symbol[counter] = line[i];
        counter++;
      }
      symbol[counter++] = '\0';
      symbolValue[curretSymbolIndex] = lineCount;
      symbolKeys[curretSymbolIndex] = symbol;
      curretSymbolIndex++;
    } else {
      lineCount++;
    }
  }

  rewind(program);
  while (fgets(line, sizeof(line), program)) {
    // verifica se é um comentário delimitado por /
    if (line[0] == '\r' || line[0] == '/' || line[0] == '(') {
      continue;
    }
    // Remove os espaços iniciais
    trim(line);
    // varre a linha atual
    for (int x = 0; line[x] != '\0'; x++) {
      // verifica se é um inline comentary
      if (line[x] == ' ' | line[x] == 13 | line[x] == 10) {
        line[x] = '\0';
      }
    }
    if (line[0] == '@') {
      char *chopped = line + 1;
      int found = 0;
      if (isNumber(chopped)) {
        int *conversion = decimalToBinary(strtol(chopped, NULL, 10));

        // fprintf(output, "%s -> ", line);
        fprintf(output, "%d", 0);
        for (int i = 0; i < (15); i++) {
          fprintf(output, "%d", conversion[i]);
        }
        fprintf(output, "\n");
        free(conversion);
      } else {
        for (int i = 0; symbolKeys[i] != NULL; i++) {
          if (strcmp(symbolKeys[i], chopped) == 0) {
            found = 1;
            int *conversion = decimalToBinary(symbolValue[i]);

            // fprintf(output, "%s -> ", line);
            fprintf(output, "%d", 0);
            for (int i = 0; i < 15; i++) {
              fprintf(output, "%d", conversion[i]);
            }
            fprintf(output, "\n");
            free(conversion);
          }
        }
        if (found == 0) {
          symbolKeys[curretSymbolIndex] = strdup(chopped);
          symbolValue[curretSymbolIndex] = variableValue;
          // printf("%s:%d\n", symbolKeys[curretSymbolIndex],
          //        symbolValue[curretSymbolIndex]);
          int *conversion = decimalToBinary(variableValue);

          // fprintf(output, "%s -> ", line);
          fprintf(output, "%d", 0);
          for (int i = 0; i < 15; i++) {
            fprintf(output, "%d", conversion[i]);
          }
          fprintf(output, "\n");
          free(conversion);
          curretSymbolIndex++;
          variableValue++;
        }
      }
    } else {
      char *comp = separarComp(line);
      char *dest = separarDest(line);
      char *jmp = separarJmp(line);
      int *compBin = traduzirComp(comp);
      int *destBin = traduzirDest(dest);
      int *jmpBin = traduzirJmp(jmp);
      char *cOutput = malloc(13 * sizeof(char));
      // fprintf(output, "%s -> ", line);
      fprintf(output, "111");
      // fprintf(output, "\n");
      // fprintf(output, "Comp-> %s:", comp);
      for (int i = 0; i < 7; i++) {
        fprintf(output, "%d", compBin[i]);
      }
      // fprintf(output, "\n");
      // fprintf(output, "Dest-> %s:", dest);
      for (int i = 0; i < 3; i++) {
        fprintf(output, "%d", destBin[i]);
      }
      // fprintf(output, "\n");
      // fprintf(output, "Jmp-> %s:", jmp);
      for (int i = 0; i < 3; i++) {
        fprintf(output, "%d", jmpBin[i]);
      }
      fprintf(output, "\n");
      free(comp);
      free(dest);
      free(compBin);
      free(destBin);
      free(jmpBin);
    }
    // for (int i = 0; symbolKeys[i] != NULL; i++) {
    //   printf("%s:%d\n", symbolKeys[i], symbolValue[i]);
    // }
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
    // shift the remainder of the string contents up by the number of
    // leading white space characters found
    int i = 0;
    while (s[i + count] != '\0') {
      s[i] = s[i + count];
      i++;
    }

    // terminate the string earlier given that the contents are being
    // reduced, the null terminator is shifted up by the number of leading
    // whitespace characters as well
    s[i] = '\0';
  }
}

int *decimalToBinary(int num) {
  int *binary = calloc(15, sizeof(int));
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
  return binary;
}

int isNumber(char *number) {
  for (int i = 0; number[i] != '\0'; i++) {
    if (!isdigit(number[i])) {
      return 0;
    }
  }
  // printf("%s\n", number);
  return 1;
}

char *separarDest(char *linha) {
  char *output = calloc(4, sizeof(char));
  if (strstr(linha, "=") != NULL) {
    for (int i = 0; linha[i] != '\0'; i++) {
      if (linha[i] == '=') {
        output[i] = '\0';
        break;
      }
      output[i] = linha[i];
    }
  }
  return output;
}

int *traduzirDest(char *dest) {
  int *output = calloc(3, sizeof(int));
  if (dest != NULL) {
    for (int i = 0; i < sizeof(dest); i++) {
      if (dest[i] == 'M') {
        output[2] = 1;
      } else if (dest[i] == 'D') {
        output[1] = 1;
      } else if (dest[i] == 'A') {
        output[0] = 1;
      }
    }
  }
  // printf("Dest:%s\n", dest);
  // printf("Trad:");
  // for (int i = 0; i < 3; i++) {
  //   printf("%d", output[i]);
  // }
  // printf("\n");
  return output;
}

char *separarComp(char *linha) {
  char *output = calloc(4, sizeof(char));
  int inicio = 0;
  int final = 0;
  int counter = 0;
  for (int i = 0; i < sizeof(linha); i++) {
    if (linha[i] == '=') {
      inicio = i + 1;
    }
    if (linha[i] == ';' || linha[i] == '\0') {
      final = i;
      break;
    }
  }
  for (int x = inicio; x <= strlen(linha); x++) {
    output[counter] = linha[x];
    if (x == final) {
      output[counter] = '\0';
      break;
    }
    counter++;
  }
  return output;
}

int *traduzirComp(char *comp) {
  int *output = calloc(7, sizeof(int));
  // printf("Comp:%s\n", comp);
  // printf("Trad:");
  // for (int i = 0; i < 7; i++) {
  //   printf("%d", output[i]);
  // }
  // printf("\n");
  if (comp != NULL) {
    if (strstr(comp, "M") != NULL) {
      output[0] = 1;
    }
    if (strcmp(comp, "0") == 0) {
      output[1] = 1;
      output[2] = 0;
      output[3] = 1;
      output[4] = 0;
      output[5] = 1;
      output[6] = 0;
    } else if (strcmp(comp, "1") == 0) {
      output[1] = 1;
      output[2] = 1;
      output[3] = 1;
      output[4] = 1;
      output[5] = 1;
      output[6] = 1;
    } else if (strcmp(comp, "-1") == 0) {
      output[1] = 1;
      output[2] = 1;
      output[3] = 1;
      output[4] = 0;
      output[5] = 1;
      output[6] = 0;
    } else if (strcmp(comp, "D") == 0) {
      output[1] = 0;
      output[2] = 0;
      output[3] = 1;
      output[4] = 1;
      output[5] = 0;
      output[6] = 0;
    } else if (strcmp(comp, "A") == 0 || strcmp(comp, "M") == 0) {
      output[1] = 1;
      output[2] = 1;
      output[3] = 0;
      output[4] = 0;
      output[5] = 0;
      output[6] = 0;
    } else if (strcmp(comp, "!D") == 0) {
      output[1] = 0;
      output[2] = 0;
      output[3] = 1;
      output[4] = 1;
      output[5] = 0;
      output[6] = 1;
    } else if (strcmp(comp, "!A") == 0 || strcmp(comp, "!M") == 0) {
      output[1] = 1;
      output[2] = 1;
      output[3] = 0;
      output[4] = 0;
      output[5] = 0;
      output[6] = 1;
    } else if (strcmp(comp, "-D") == 0) {
      output[1] = 0;
      output[2] = 0;
      output[3] = 1;
      output[4] = 1;
      output[5] = 1;
      output[6] = 1;
    } else if (strcmp(comp, "-A") == 0 || strcmp(comp, "-M") == 0) {
      output[1] = 1;
      output[2] = 1;
      output[3] = 0;
      output[4] = 0;
      output[5] = 1;
      output[6] = 1;
    } else if (strcmp(comp, "D+1") == 0) {
      output[1] = 0;
      output[2] = 1;
      output[3] = 1;
      output[4] = 1;
      output[5] = 1;
      output[6] = 1;
    } else if (strcmp(comp, "A+1") == 0 || strcmp(comp, "M+1") == 0) {
      output[1] = 1;
      output[2] = 1;
      output[3] = 0;
      output[4] = 1;
      output[5] = 1;
      output[6] = 1;
    } else if (strcmp(comp, "D-1") == 0) {
      output[1] = 0;
      output[2] = 0;
      output[3] = 1;
      output[4] = 1;
      output[5] = 1;
      output[6] = 0;
    } else if (strcmp(comp, "A-1") == 0 || strcmp(comp, "M-1") == 0) {
      output[1] = 1;
      output[2] = 1;
      output[3] = 0;
      output[4] = 0;
      output[5] = 1;
      output[6] = 0;
    } else if (strcmp(comp, "D+A") == 0 || strcmp(comp, "D+M") == 0) {
      output[1] = 0;
      output[2] = 0;
      output[3] = 0;
      output[4] = 0;
      output[5] = 1;
      output[6] = 0;
    } else if (strcmp(comp, "D-A") == 0 || strcmp(comp, "D-M") == 0) {
      output[1] = 0;
      output[2] = 1;
      output[3] = 0;
      output[4] = 0;
      output[5] = 1;
      output[6] = 1;
    } else if (strcmp(comp, "A-D") == 0 || strcmp(comp, "M-D") == 0) {
      output[1] = 0;
      output[2] = 0;
      output[3] = 0;
      output[4] = 1;
      output[5] = 1;
      output[6] = 1;
    } else if (strcmp(comp, "D&A") == 0 || strcmp(comp, "D&M") == 0) {
      output[1] = 0;
      output[2] = 0;
      output[3] = 0;
      output[4] = 0;
      output[5] = 0;
      output[6] = 0;
    } else if (strcmp(comp, "D|A") == 0 || strcmp(comp, "D|M") == 0) {
      output[1] = 0;
      output[2] = 1;
      output[3] = 0;
      output[4] = 1;
      output[5] = 0;
      output[6] = 1;
    }
  }

  // for (int i = 0; i < 7; i++) {
  //   printf("%d", output[i]);
  // }
  // printf("\n-----------------------------\n");
  return output;
}

char *separarJmp(char *linha) {
  for (int i = 0; linha[i] != '\0'; i++) {
    if (linha[i] == ';') {
      return linha + i + 1;
    }
  }
  return NULL;
}

int *traduzirJmp(char *jmp) {
  int *output = calloc(3, sizeof(int));
  if (jmp != NULL) {
    if (jmp[2] == 'E' || jmp[1] == 'E') {
      output[1] = 1;
    }
    if (jmp[1] == 'G') {
      output[2] = 1;
    }
    if (jmp[1] == 'L') {
      output[0] = 1;
    }
    if (jmp[1] == 'N') {
      output[0] = 1;
      output[1] = 0;
      output[2] = 1;
    }
    if (jmp[1] == 'M') {
      output[0] = 1;
      output[1] = 1;
      output[2] = 1;
    }
    // if (strcmp("JMP", jmp) != 0) {
    //   printf("jmp:%s\n", jmp);
    //   printf("Trad:");
    //
    //   for (int i = 0; i < 3; i++) {
    //     printf("%d", output[i]);
    //   }
    //   printf("\n");
    // }
  }
  return output;
}

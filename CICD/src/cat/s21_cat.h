#ifndef S21_CAT
#define S21_CAT

#define BUFFER 4096

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkFlag(int argc, char **argv);
void readFile(int argc, char **argv, int *flag, int curArg);
void printFile(char *str, int *flag, FILE *fp);
void printChar(unsigned char symbol, int flagV);
void printError(char *opt);

typedef enum EFLAGS { B, E, N, S, T, V, ERROR } FLAGS;

#endif
#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    checkFlag(argc, argv);
  } else {
    printf("Non argument or option in cat");
  }
  return 0;
}

void checkFlag(int argc, char **argv) {
  int argFlag[7] = {0};
  memset(&argFlag, 0, sizeof(int));
  int isFlag = 1;
  int curArg = 1;
  for (int i = 1; i < argc; i++) {
    if (isFlag) {
      if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--number-nonblank")) {
        argFlag[B] = 1;
      } else if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "-E")) {
        argFlag[E] = 1;
        if (!strcmp(argv[i], "-e")) {
          argFlag[V] = 1;
        }
      } else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--number")) {
        argFlag[N] = 1;
      } else if (!strcmp(argv[i], "-s") ||
                 !strcmp(argv[i], "--squeeze-blank")) {
        argFlag[S] = 1;
      } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "-T")) {
        argFlag[T] = 1;
        if (!strcmp(argv[i], "-t")) {
          argFlag[V] = 1;
        }
      } else if (!strcmp(argv[i], "-v")) {
        argFlag[V] = 1;
      } else if (argv[i][0] == '-') {
        argFlag[ERROR] = 1;
        printError(argv[i]);
      } else {
        curArg = i;
        isFlag = 0;
      }
    }
  }
  if (argFlag[ERROR] != 1) {
    readFile(argc, argv, argFlag, curArg);
  }
}

void readFile(int argc, char **argv, int *flag, int curArg) {
  char *DIR;
  FILE *fp;
  char str[BUFFER] = {0};
  for (int i = curArg; i < argc; i++) {
    DIR = argv[i];
    if ((fp = fopen(DIR, "r")) == NULL) {
      printf("%s: No such file or directory\n", DIR);
    } else {
      printFile(str, flag, fp);
      fclose(fp);
    }
  }
}

void printFile(char *str, int *flag, FILE *fp) {
  int curLine = 1;
  int isSpace = 0;
  while (fgets(str, BUFFER, fp) != NULL) {
    if (flag[B] && str[0] != '\n') {
      printf("%6d\t", curLine);
      curLine++;
    }
    if (flag[S]) {
      if (!isSpace && str[0] == '\n') {
        isSpace = 1;
      } else if (isSpace && str[0] == '\n') {
        continue;
      } else if (isSpace && str[0] != '\n') {
        isSpace = 0;
      }
    }
    if (flag[N] && !flag[B]) {
      printf("%6d\t", curLine);
      curLine++;
    }
    for (size_t i = 0; i < strlen(str); i++) {
      if (flag[T] && str[i] == '\t') {
        printf("^");
        str[i] += 64;
      }
      if (flag[E] && str[i] == '\n' && flag[B] && i == 0) {
        printf("      \t$");
      } else if (flag[E] == 1 && str[i] == '\n') {
        printf("$");
      }
      printChar(str[i], flag[V]);
    }
  }
}

void printChar(unsigned char symbol, int flagV) {
  if (flagV == 1) {
    if (symbol < 32 && symbol != '\t' && symbol != '\n') {
      symbol += 64;
      printf("^%c", symbol);
    } else if (symbol == 127) {
      printf("^?");
    } else if (symbol > 127 && symbol < 160) {
      symbol -= 64;
      printf("M-^%c", symbol);
    } else {
      printf("%c", symbol);
    }
  } else {
    printf("%c", symbol);
  }
}

void printError(char *opt) {
  printf("cat: invalid option -- ");
  int lenOpt = strlen(opt);
  for (int i = 1; i < lenOpt; i++) {
    printf("%c", opt[i]);
  }
}

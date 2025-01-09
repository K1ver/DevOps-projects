#ifndef S21_GREP
#define S21_GREP

#define BUFFER 4096

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  int e, i, v, c, l, n, h, s, f, o;
} opt;

void printError();
void checkFlag(int argc, char **argv);
regex_t createReg(char *pattern, opt *options);
void readPatternFile(char *dir, char *templateStr, opt *options);
void setPattern(char *templateStr, char *templateArg, opt *options);
void readFile(int argc, char **argv, opt *options, char *templatesStr);
void printFile(opt *options, int count_f, char *dir, FILE *fp, regex_t preg);
void parser(int argc, char **argv, opt *options, char *templatesStr, int *flag);
void FlagO(opt *options, int count_f, char *dir, int curLine, char *str,
           regex_t preg, regmatch_t pmatch);

#endif
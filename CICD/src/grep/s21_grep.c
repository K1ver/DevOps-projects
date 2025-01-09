#include "s21_grep.h"

int main(int argc, char **argv) {
  opt option = {0};
  int checkFlag = 0;
  char templatesStr[BUFFER] = {0};
  if (argc > 1) {
    parser(argc, argv, &option, templatesStr, &checkFlag);
    if (!option.f && !option.e) {
      for (int i = optind; i < argc; i++) {
        if (strlen(templatesStr) == 0) {
          setPattern(templatesStr, argv[i], &option);
        }
      }
    }
    if (!checkFlag) {
      readFile(argc, argv, &option, templatesStr);
    }
  } else {
    printError();
  }
  return 0;
}

void parser(int argc, char **argv, opt *options, char *templatesStr,
            int *flag) {
  int opt = 0;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1 &&
         *flag == 0) {
    switch (opt) {
      case 'e':
        options->e = 1;
        setPattern(templatesStr, optarg, options);
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        readPatternFile(optarg, templatesStr, options);
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        printError(opt);
        *flag = 1;
        break;
    }
  }
}

void setPattern(char *templateStr, char *templateArg, opt *options) {
  if (options->e || options->f) {
    if (strlen(templateStr) == 0) {
      if (strchr(templateArg, '\n')) {
        strncat(templateStr, templateArg, strlen(templateArg) - 1);
      } else {
        strcat(templateStr, templateArg);
      }
    } else {
      strcat(templateStr, "|");
      if (strchr(templateArg, '\n')) {
        strncat(templateStr, templateArg, strlen(templateArg) - 1);
      } else {
        strcat(templateStr, templateArg);
      }
    }
  } else {
    strcat(templateStr, templateArg);
  }
}

void readFile(int argc, char **argv, opt *options, char *templatesStr) {
  char *DIR;
  FILE *fp;
  if (!options->e && !options->f) {
    optind++;
  }
  int count_f = argc - optind;
  regex_t preg = createReg(templatesStr, options);
  for (int i = optind; i < argc; i++) {
    DIR = argv[i];
    if (argv[i - 1][0] == '-' &&
        (strchr(argv[i - 1], 'f') || strchr(argv[i - 1], 'e'))) {
      continue;
    }
    if ((fp = fopen(DIR, "r")) == NULL) {
      if (!options->s) {
        printf("grep: %s: No such file or directory\n", DIR);
      }
    } else {
      printFile(options, count_f, DIR, fp, preg);
      fclose(fp);
    }
  }
  regfree(&preg);
}

void printFile(opt *options, int count_f, char *dir, FILE *fp, regex_t preg) {
  char str[BUFFER] = {0};
  int curLine = 0;
  int match = 0;
  while (fgets(str, BUFFER - 2, fp) != NULL) {
    regmatch_t pmatch;
    int value;
    value = regexec(&preg, str, 1, &pmatch, 0);
    if (!strchr(str, '\n')) {
      int len = strlen(str);
      str[len] = '\n';
      str[len + 1] = '\0';
    }
    if (!options->c) {
      curLine++;
    }
    if (value == 0) {
      match = 1;
    }
    if (value == 0 && options->o && !options->c && !options->l && !options->v) {
      FlagO(options, count_f, dir, curLine, str, preg, pmatch);
    } else if (((value == 1 && options->v) || (value == 0 && !options->v)) &&
               (!options->c && !options->l)) {
      if (count_f > 1 && !options->h) {
        printf("%s:", dir);
      }
      if (options->n) {
        printf("%d:", curLine);
      }
      printf("%s", str);
    } else if ((value == 0 && options->c && !options->v) ||
               (value == 1 && options->c && options->v)) {
      curLine++;
    }
  }
  if (options->c) {
    if (count_f > 1 && !options->h) {
      printf("%s:", dir);
    }
    if (options->l) {
      printf("1\n");
    } else {
      printf("%d\n", curLine);
    }
  }
  if (match && options->l) {
    printf("%s\n", dir);
  }
}

void FlagO(opt *options, int count_f, char *dir, int curLine, char *str,
           regex_t preg, regmatch_t pmatch) {
  char *copy;
  copy = str;
  while (copy && 0 == (regexec(&preg, copy, 1, &pmatch, 0))) {
    if (count_f > 1 && !options->h) {
      printf("%s:", dir);
    }
    if (options->n) {
      printf("%d:", curLine);
    }
    int i = pmatch.rm_so;
    while (i < pmatch.rm_eo) {
      printf("%c", copy[i]);
      i++;
    }
    printf("\n");
    copy = copy + pmatch.rm_eo;
  }
}

regex_t createReg(char *pattern, opt *options) {
  regex_t preg;
  if (options->i) {
    regcomp(&preg, pattern, REG_EXTENDED | REG_ICASE);
  } else {
    regcomp(&preg, pattern, REG_EXTENDED);
  }
  return preg;
}

void readPatternFile(char *dir, char *templateStr, opt *options) {
  FILE *fp;
  char str[BUFFER];
  if ((fp = fopen(dir, "r")) == NULL) {
    if (!options->s) {
      printf("%s: No such file or directory\n", dir);
    }
  } else {
    while (fgets(str, BUFFER, fp) != NULL) {
      setPattern(templateStr, str, options);
    }
    fclose(fp);
  }
}

void printError() { printf("usage: s21_grep [eivclnhsfo] [file ...]\n"); }
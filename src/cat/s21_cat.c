#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} opt;

int parser(int argc, char *argv[], opt *options);
void reader(char *argv[], opt *options);

int main(int argc, char *argv[]) {
  opt options = {0};
  int flag = 0;
  flag = parser(argc, argv, &options);
  if (argc == 1 || (argc == 2 && flag != 0)) {
    while (1) {
      fprintf(stdout, "%c", fgetc(stdin));
    }
  } else {
    while (optind < argc) {
      if ((argc >= 2 || (argc >= 3 && flag)) && argv[optind] != NULL) {
        reader(argv, &options);
      } else {
        printf("ERROR\n");
      }
      optind++;
    }
  }
  return 0;
}
int parser(int argc, char *argv[], opt *options) {
  int opt = 0;
  int flag = 0;  //проверка на наличие флагов
  int option_index = 0;
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}};
  while ((opt = getopt_long(argc, argv, "+benstvTE", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        flag++;
        break;
      case 'n':
        options->n = 1;
        flag++;
        break;
      case 'v':
        options->v = 1;
        flag++;
        break;
      case 's':
        options->s = 1;
        flag++;
        break;
      case 'T':
        options->t = 1;
        flag++;
        break;
      case 'E':
        options->e = 1;
        flag++;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        flag++;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        flag++;
        break;
      default:
        printf("error\n");
        break;
    };
    if (options->b && options->n) options->n = 0;
  }
  return flag;
}
void reader(char *argv[], opt *options) {
  FILE *f = NULL;
  int nflag = 0;  //для понимания первого символа файла
  f = fopen(argv[optind], "r");
  int cur, ver = 1, mep = 0, count = 1, first = 1;
  if (f == NULL) {
    fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
            argv[optind]);
  } else {
    while ((cur = fgetc(f)) != EOF) {
      if (options->s) {
        if (cur == '\n') mep++;
        if (cur != '\n') mep = 0;
      }
      if (cur == '\n' && (!options->s || mep < 3)) {
        if ((options->n && ver == '\n' && cur) || (options->n && nflag == 0))
          printf("%6d\t", count++);
        if (options->e) printf("$");
        printf("%c", cur);
      } else if (cur != '\n') {
        if ((ver == '\n' || first == 1) && (options->n || options->b))
          printf("%6d\t", count++);
        if (cur < 32 && cur != 9 && cur != 10 && options->v)
          printf("^%c", cur + 64);
        else if (cur > 127 && cur < 160 && options->v)
          printf("M-^%c", cur - 64);
        else if (cur == 127 && options->v)
          printf("^%c", cur - 64);
        else if (cur == '\t' && options->t)
          printf("^I");
        else
          printf("%c", cur);
      }
      ver = cur;
      first = 0;
      nflag++;
    }
    fclose(f);
  }
}
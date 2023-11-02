#include "s21_grep.h"

int main(int argc, char *argv[]) {
  FLAGS flag = {0};
  char *buf[1024] = {0};
  int flg = check_flags(argc, argv, &flag, buf);
  if (argc <= 2 || flg == -1)  //ошибка без файла
    printf(
        "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
        "[-C[num]]");
  if (flg == -3) {  //если е или f не верно сработали
    printf("grep: option requires an argument -- e");
  }

  get_pattern(argv, argc, buf, &flag);  //принимает шаблон
  int d = 0;
  while (buf[d] != NULL) {
    d++;
  }
  int c_files = 0;
  int i = 1;  //колличество файлов
  while (i < argc) {
    if (argv[i][0] != '\0') {
      c_files++;
    }
    i++;
  }
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '\0') {
      if (read_file(argv[i], buf, c_files, &flag) == 0 ||
          flg == -2) {  // если файл пуст или отсутствует
        if (flag.s != 1)
          fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      }
    }
  }

  for (int i = 0; i < d; i++) free(buf[i]);

  return 0;
}
int check_flags(int argc, char **argv, FLAGS *flag, char **buf) {
  int pattern = 0;  //
  int flagi = 0;
  int i = 1;  //индекс строки
  while (i < argc) {
    if (argv[i][0] == '-') {
      int j = 1;  //индекс символа
      while (argv[i][j]) {
        switch (argv[i][j]) {
          case 'f':
            flag->f = 1;
            flagi = get_pattern_ef(argv, buf, pattern, i, j);
            flagi = flag_f(buf, pattern);
            flagi++;
            pattern++;
            break;
          case 'e':
            flag->e = 1;
            flagi = get_pattern_ef(argv, buf, pattern, i, j);
            flagi++;
            pattern++;
            break;
          case 'i':
            flag->i = 1;
            flagi++;
            break;
          case 'v':
            flag->v = 1;
            flagi++;
            break;
          case 'c':
            flag->c = 1;
            flagi++;
            break;
          case 'l':
            flag->l = 1;
            flagi++;
            break;
          case 'n':
            flag->n = 1;
            flagi++;
            break;
          case 'h':
            flag->h = 1;
            flagi++;
            break;
          case 's':
            flag->s = 1;
            flagi++;
            break;
          case 'o':
            flag->o = 1;
            flagi++;
            break;
          default:
            flagi = -1;
            break;
        }
        j++;
      }
      memset(argv[i], '\0',
             strlen(argv[i]));  //меняет аргумент на пустую строкуб пока строка
                                //не закончится
    }
    i++;
  }
  return flagi;
}
int get_pattern_ef(char **argv, char **buf, int pattern, int i, int j) {
  int flag = 0;
  if (argv[i][j + 1] != '\0') {  //шаблон записан с флагом
    buf[pattern] = (char *)malloc(sizeof(char) * (strlen(argv[i]) + 1));
    if (buf[pattern] != NULL) {
      strcpy(buf[pattern], argv[i]);
      memset(argv[i], '\0', strlen(argv[i]));
      read_ef(buf[pattern], j);  //он разделяяет флаг от шаблона
    } else
      flag = -1;
  } else if (argv[i + 1] !=
             NULL) {  //если раздельно написан - удаление следующего
    buf[pattern] = (char *)malloc(sizeof(char) * (strlen(argv[i + 1]) + 1));
    if (buf[pattern] != NULL) {  //после флага слово - копируем в буф
      strcpy(buf[pattern], argv[i + 1]);
      memset(argv[i + 1], '\0', strlen(argv[i + 1]));
    } else
      flag = -1;
  } else
    flag = -3;
  return flag;
}

void get_pattern(char **argv, int argc, char **buf, FLAGS *flag) {
  int stop = 0;
  if (flag->e != 1 && flag->f != 1) {
    int i = 1;
    while (i < argc && stop != 1) {
      if (argv[i][0] != '\0') {
        buf[0] = (char *)malloc(sizeof(char) * (strlen(argv[i]) + 1));
        if (buf[0] != NULL) {
          strcpy(buf[0], argv[i]);
          memset(argv[i], '\0', strlen(argv[i]));
          stop = 1;
          // break;
        }
      }
      i++;
    }
  }
}
void read_ef(char *buf, int c) {
  if (strlen(buf) - c > 0) {
    char temp[strlen(buf) - c];  //объявление массива для сохранения строки
    int j = 0;
    for (int i = c + 1; buf[i] != '\0'; i++) {
      temp[j] = buf[i];
      j++;
    }
    temp[j] = '\0';
    strcpy(buf, temp);  //сохранение в буф
  }
}
int read_file(char *name_file, char **search, int d, FLAGS *flag) {
  char *str = NULL;
  int flag_ = 1;
  int flag_c = 0;
  size_t len = 0;
  FILE *fi;
  //проверка на наличиее шаблонов в строке

  fi = fopen(name_file, "r");
  if (fi != NULL) {
    int i = 1;
    while (getline(&str, &len, fi) != -1) {
      if (len != 0) {
        int chesk = 0;
        int curr_pat = 0;
        while (search[curr_pat] != NULL) {
          grep(str, search[curr_pat], &chesk, flag);
          curr_pat++;
        }
        if (flag->v == 1) flag_v(&chesk);
        if (chesk != 0) {
          flag_c++;
          if (flag->c != 1 && flag->l != 1 && chesk != 0)
            output(str, flag, name_file, i,
                   d);  //вызывается пр  наличии шаблонов в строке
        }
        i++;
      }
    }
    if (flag->c == 1 || flag->l == 1) out_for_c_l_(flag, d, flag_c, name_file);
    fclose(fi);
  } else {  // выводит ошибку No such file or directory
    flag_ = 0;
  }
  free(str);
  return flag_;
}
void grep(char *str, char *pattern, int *match_no, FLAGS *flag) {
  int reg = 0;
  regex_t reegex;  //для компилирования строк
  if (strcmp(pattern, ".") == 0)
    reg = regcomp(&reegex, pattern, REG_NEWLINE);
  else {
    reg = regcomp(&reegex, pattern,  //компилирует в тип regex_t и сохраняет
                  (flag->i == 1 ? REG_ICASE | REG_EXTENDED : REG_EXTENDED));
  }
  if (reg == 0) {
    if (regexec(&reegex, str, 0, NULL, 0) ==
        0) {  //сохраненный в ссылкпе паттерн ищет в str
      *match_no = 1;
    }
  }
  regfree(&reegex);
}
void output(char *str, FLAGS *flag, char *name_file, int i, int c_files) {
  if (flag->h != 1 && c_files > 1) fprintf(stdout, "%s:", name_file);
  if (flag->n == 1) fprintf(stdout, "%d:", i);
  fprintf(stdout, "%s", str);
  if (strstr(str, "\n") == NULL) fprintf(stdout, "\n");
}
void out_for_c_l_(FLAGS *flag, int c_files, int matches, char *name_file) {
  if (c_files > 1 && flag->c == 1 && flag->h != 1)
    fprintf(stdout, "%s:", name_file);
  if (flag->c == 1 && flag->l == 1 && matches > 0) matches = 1;
  if (flag->c == 1) fprintf(stdout, "%d\n", matches);
  if (flag->l == 1 && matches > 0) fprintf(stdout, "%s\n", name_file);
}

int flag_f(char **buf, int _ind_) {
  int err = 1;
  char *temp = NULL;
  size_t len = 0;
  FILE *fp;
  fp = fopen(buf[_ind_], "r");
  if (fp != NULL) {
    free(buf[_ind_]);
    while (getline(&temp, &len, fp) != -1) {
      buf[_ind_] = (char *)malloc(sizeof(char) * (strlen(temp) + 1));
      if (buf[_ind_] != NULL) {
        strcpy(buf[_ind_], temp);
        int _len = strlen(buf[_ind_]);
        if (buf[_ind_][_len - 1] == '\n' && strlen(buf[_ind_]) > 1)
          buf[_ind_][_len - 1] = '\0';
        _ind_++;
      }
    }
    fclose(fp);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", buf[_ind_]);
    err = -2;
  }

  free(temp);
  return err;
}
void flag_v(int *chesk) { *chesk = *chesk == 0 ? 1 : 0; }

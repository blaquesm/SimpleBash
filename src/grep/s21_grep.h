#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FLAGS {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int t;
  int f;
  int o;
} FLAGS;
int check_flags(int argc, char **argv, FLAGS *flag, char **buf);
int get_pattern_ef(char **argv, char **buf, int pattern, int i, int j);
int flag_f(char **buf, int pattern);
void get_pattern(char **argv, int argc, char **buf, FLAGS *flag);
int get_file(char **argv, int argc, int *count_files);
int read_file(char *name_file, char **search, int d, FLAGS *flag);
void read_ef(char *buf, int c);
void grep(char *str, char *pattern, int *match_no, FLAGS *flag);
void output(char *str, FLAGS *flag, char *name_file, int i, int c_files);
void out_for_c_l_(FLAGS *flag, int c_files, int matches, char *name_file);
int flag_f(char **buf, int _ind_);
void flag_v(int *chesk);
#endif


CC = gcc
CFLAG = -Wall -Werror -Wextra



all : s21_grep 

clean :
	rm -rf *.o
	rm -rf s21_grep

rebuild : clean all

s21_grep: 
	$(CC) $(CFLAG) -c s21_grep.c
	$(CC) $(CFLAG) -o s21_grep *.o 

test :
	sh test_func_grep.sh
style:
	cp ../../materials/build/scripts/.clang-format ../grep
	clang-format -n s21_grep.c s21_grep.h
	rm -rf .clang-format
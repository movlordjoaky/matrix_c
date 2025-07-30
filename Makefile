CC = gcc
FLAG_C = -c
FLAG_O = -o 
FLAG_ER = -Wall -Werror -Wextra -std=c11
# FLAG_ER = 
FLAG_TESTS = -lcheck -lm -lsubunit -lpthread
s21_MATRIX_C = s21_*.c 
s21_MATRIX_O = s21_*.o

all: clean s21_matrix.a

s21_matrix.a:
	$(CC) $(FLAG_C) $(FLAG_ER) $(s21_MATRIX_C)
	ar rcs s21_matrix.a $(s21_MATRIX_O)

clean:
	-rm -rf *.o s21_matrix.a program* report *.gcno *.gcda leaks.txt

main:
	$(CC) $(s21_MATRIX_C) main.c -o program
	./program

test: clean s21_matrix.a
	$(CC) $(FLAG_C) $(FLAG_ER) $(s21_MATRIX_C) --coverage
	$(CC) $(FLAG_ER) --coverage $(s21_MATRIX_C) test.c $(FLAG_TESTS) -o program
	./program

gcov_report: test
	mkdir -p report/coverage_data
	lcov  --directory . --capture --output-file coverage.info
	lcov --remove coverage.info "test.c" -o coverage.info
	genhtml coverage.info --output-directory report
	mv *.gcno report/coverage_data
	mv *.gcda report/coverage_data
	mv coverage.info report/coverage_data

open:
	start report/index.html

leaks: test
	-rm -rf leaks.txt
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./program > leaks.txt 2>&1
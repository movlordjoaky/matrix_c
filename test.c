#include <check.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

#define EPS 1e-6

void print_m(matrix_t *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->columns; j++) {
      printf("%3.3g ", m->matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void print_v(int *p, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", p[i]);
  }
  printf("\n");
}

void init_m(int rows, int columns, matrix_t *m, ...) {
  va_list args;
  va_start(args, m);
  s21_create_matrix(rows, columns, m);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      m->matrix[i][j] = (double)va_arg(args, int);
    }
  }
  va_end(args);
}

void init(matrix_t *m) {
  m->rows = 0;
  m->columns = 0;
  m->matrix = NULL;
}

START_TEST(test_create) {
  int r = 2, c = 2, res;
  matrix_t m;
  res = s21_create_matrix(r, c, &m);
  ck_assert_int_eq(m.rows, r);
  ck_assert_int_eq(m.columns, c);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      ck_assert_int_eq(m.matrix[i][j], 0);
    }
  }
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_create_zero_dimension) {
  int r = 2, c = 0, res;
  matrix_t m;
  res = s21_create_matrix(r, c, &m);
  ck_assert_int_eq(res, 1);
  // LCOV_EXCL_START
  s21_remove_matrix(&m);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_remove) {
  int r = 2, c = 2;
  matrix_t m;
  // LCOV_EXCL_START
  s21_create_matrix(r, c, &m);
  // LCOV_EXCL_STOP
  ck_assert_ptr_nonnull(m.matrix);
  s21_remove_matrix(&m);
  ck_assert_ptr_null(m.matrix);
}
END_TEST

START_TEST(test_eq) {
  matrix_t m1, m2;
  init_m(2, 3, &m1, 1, 2, 3, 4, 5, 6);
  init_m(2, 3, &m2, 1, 2, 3, 4, 5, 6);
  int res = s21_eq_matrix(&m1, &m2);
  ck_assert_int_eq(res, SUCCESS);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_eq_zero_dimension) {
  int r = 2, c = 2;
  matrix_t m1, m2;
  // LCOV_EXCL_START
  s21_create_matrix(r, c, &m1);
  s21_create_matrix(r, c, &m2);
  // LCOV_EXCL_STOP
  m2.columns = -2;
  int res = s21_eq_matrix(&m1, &m2);
  ck_assert_int_eq(res, FAILURE);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_sum) {
  matrix_t m1, m2, m3, m4;
  init_m(2, 3, &m1, 1, 2, 3, 4, 5, 6);
  init_m(2, 3, &m2, 1, 2, 3, 4, 5, 6);
  init(&m3);
  init_m(2, 3, &m4, 2, 4, 6, 8, 10, 12);
  int res_sum = s21_sum_matrix(&m1, &m2, &m3);
  ck_assert_int_eq(res_sum, 0);
  // LCOV_EXCL_START
  int res_eq = s21_eq_matrix(&m3, &m4);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m3);
  s21_remove_matrix(&m4);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_sub) {
  matrix_t m1, m2, m3, m4;
  init_m(2, 3, &m1, 2, 4, 6, 8, 10, 12);
  init_m(2, 3, &m2, 1, 2, 3, 4, 5, 6);
  init(&m3);
  init_m(2, 3, &m4, 1, 2, 3, 4, 5, 6);
  int res_sub = s21_sub_matrix(&m1, &m2, &m3);
  ck_assert_int_eq(res_sub, 0);
  // LCOV_EXCL_START
  int res_eq = s21_eq_matrix(&m3, &m4);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m3);
  s21_remove_matrix(&m4);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_mult_number) {
  matrix_t m1, m2, m3;
  init_m(1, 2, &m1, 1, 2);
  init(&m2);
  init_m(1, 2, &m3, 2, 4);
  int res_mult = s21_mult_number(&m1, 2, &m2);
  ck_assert_int_eq(res_mult, 0);
  // LCOV_EXCL_START
  int res_eq = s21_eq_matrix(&m2, &m3);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m3);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_mult_matrix) {
  matrix_t m1, m2, m3, m4;
  init_m(3, 2, &m1, 1, 4, 2, 5, 3, 6);
  init_m(2, 3, &m2, 1, -1, 1, 2, 3, 4);
  init(&m3);
  init_m(3, 3, &m4, 9, 11, 17, 12, 13, 22, 15, 15, 27);
  int res_mult = s21_mult_matrix(&m1, &m2, &m3);
  ck_assert_int_eq(res_mult, 0);
  // LCOV_EXCL_START
  int res_eq = s21_eq_matrix(&m3, &m4);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m3);
  s21_remove_matrix(&m4);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_transpose) {
  matrix_t m1, m2, m3;
  init_m(3, 2, &m1, 1, 4, 2, 5, 3, 6);
  init(&m2);
  init_m(2, 3, &m3, 1, 2, 3, 4, 5, 6);
  int res_mult = s21_transpose(&m1, &m2);
  ck_assert_int_eq(res_mult, 0);
  // LCOV_EXCL_START
  int res_eq = s21_eq_matrix(&m2, &m3);
  ck_assert_int_eq(res_eq, SUCCESS);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m3);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant) {
  matrix_t m1;
  double det_res, exp_res = 27;
  init_m(3, 3, &m1, 1, 2, 3, 4, 5, 6, 7, 8, 0);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_2) {
  matrix_t m1;
  double det_res, exp_res = 13;
  init_m(3, 3, &m1, 0, 2, 1, 1, -1, 2, 3, 0, 1);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_3) {
  matrix_t m1;
  double det_res, exp_res = 5;
  init_m(1, 1, &m1, 5);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_4) {
  matrix_t m1;
  double det_res, exp_res = 0;
  init_m(3, 3, &m1, 1, 2, 3, 2, 4, 6, 3, 6, 9);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_5) {
  matrix_t m1;
  double det_res, exp_res = 1;
  init_m(3, 3, &m1, 1, 0, 0, 0, 1, 0, 0, 0, 1);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_6) {
  matrix_t m1;
  double det_res, exp_res = -1;
  init_m(2, 2, &m1, 0, 1, 1, 0);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_bad_matrix) {
  matrix_t m1;
  double det_res;
  init_m(2, 3, &m1, 1, 2, 3, 2, 4, 6);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_int_eq(res, 2);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_null) {
  double det_res;
  int res = s21_determinant(NULL, &det_res);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_determinant_null_2) {
  matrix_t m1;
  double det_res;
  m1.matrix = NULL;
  int res = s21_determinant(&m1, &det_res);
  ck_assert_int_eq(res, 1);
  s21_remove_matrix(&m1);
}
END_TEST

START_TEST(test_determinant_4x4) {
  matrix_t m1;
  double det_res, exp_res = 0;
  init_m(4, 4, &m1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_fractional) {
  matrix_t m1;
  double det_res, exp_res = 2.25;
  init_m(2, 2, &m1, 0, 0, 0, 0);
  m1.matrix[0][0] = 1.5;
  m1.matrix[1][1] = 1.5;
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_determinant_1x1_zero) {
  matrix_t m1;
  double det_res, exp_res = 0;
  init_m(1, 1, &m1, 0);
  int res = s21_determinant(&m1, &det_res);
  ck_assert_double_eq_tol(det_res, exp_res, EPS);
  ck_assert_int_eq(res, 0);
  // LCOV_EXCL_START
  s21_remove_matrix(&m1);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_calc_complements) {
  matrix_t m1, m2, m3;
  init_m(3, 3, &m1, 1, 2, 3, 0, 4, 2, 5, 2, 1);
  init(&m2);
  init_m(3, 3, &m3, 0, 10, -20, 4, -14, 8, -8, -2, 4);
  int res = s21_calc_complements(&m1, &m2);
  // LCOV_EXCL_START
  ck_assert_int_eq(s21_eq_matrix(&m2, &m3), SUCCESS);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m3);
  // LCOV_EXCL_STOP
}
END_TEST

START_TEST(test_inverse) {
  matrix_t m1, m2, m3;
  init_m(3, 3, &m1, 2, 5, 7, 6, 3, 4, 5, -2, -3);
  init(&m2);
  init_m(3, 3, &m3, 1, -1, 1, -38, 41, -34, 27, -29, 24);
  int res = s21_inverse_matrix(&m1, &m2);
  // LCOV_EXCL_START
  ck_assert_int_eq(s21_eq_matrix(&m2, &m3), SUCCESS);
  ck_assert_int_eq(res, 0);
  s21_remove_matrix(&m1);
  s21_remove_matrix(&m2);
  s21_remove_matrix(&m3);
  // LCOV_EXCL_STOP
}
END_TEST

Suite *string_suite(void) {
  Suite *suite = suite_create("Matrix");
  TCase *tcase = tcase_create("matrix_functions");

  tcase_add_test(tcase, test_create);
  tcase_add_test(tcase, test_create_zero_dimension);

  tcase_add_test(tcase, test_remove);

  tcase_add_test(tcase, test_eq);
  tcase_add_test(tcase, test_eq_zero_dimension);

  tcase_add_test(tcase, test_sum);

  tcase_add_test(tcase, test_sub);

  tcase_add_test(tcase, test_mult_number);

  tcase_add_test(tcase, test_mult_matrix);

  tcase_add_test(tcase, test_transpose);

  tcase_add_test(tcase, test_determinant);
  tcase_add_test(tcase, test_determinant_2);
  tcase_add_test(tcase, test_determinant_3);
  tcase_add_test(tcase, test_determinant_4);
  tcase_add_test(tcase, test_determinant_5);
  tcase_add_test(tcase, test_determinant_6);
  tcase_add_test(tcase, test_determinant_bad_matrix);
  tcase_add_test(tcase, test_determinant_null);
  tcase_add_test(tcase, test_determinant_null_2);
  tcase_add_test(tcase, test_determinant_4x4);
  tcase_add_test(tcase, test_determinant_fractional);
  tcase_add_test(tcase, test_determinant_1x1_zero);

  tcase_add_test(tcase, test_calc_complements);

  tcase_add_test(tcase, test_inverse);

  suite_add_tcase(suite, tcase);

  return suite;
}

int main(void) {
  int failed = 0;
  Suite *suite = string_suite();
  SRunner *srunner = srunner_create(suite);
  srunner_run_all(srunner, CK_NORMAL);
  failed = srunner_ntests_failed(srunner);
  srunner_free(srunner);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#include <math.h>

#include "s21_matrix.h"

#define EPS 1e-7

static int check_eq_dim(matrix_t *A, matrix_t *B);
static int cycle_eq_matrix(matrix_t *A, matrix_t *B);
static int cycle_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
static int cycle_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
static int cycle_mult_number_matrix(matrix_t *A, double number,
                                    matrix_t *result);
static int cycle_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
double calc_mul_elem(matrix_t *A, matrix_t *B, int i, int j);

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int error = check_bad_matrix(A) || check_bad_matrix(B);
  if (!error) {
    error = check_eq_dim(A, B);
  }
  if (!error) {
    error = cycle_eq_matrix(A, B);
  }
  return error ? FAILURE : SUCCESS;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = check_bad_matrix(A) || check_bad_matrix(B);
  if (!error) {
    error = check_eq_dim(A, B);
  }
  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
  }
  if (!error) {
    error = cycle_sum_matrix(A, B, result);
  }
  return error;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = check_bad_matrix(A) || check_bad_matrix(B);
  if (!error) {
    error = check_eq_dim(A, B);
  }
  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
  }
  if (!error) {
    error = cycle_sub_matrix(A, B, result);
  }
  return error;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int error = check_bad_matrix(A);
  if (!error) {
    error = s21_create_matrix(A->rows, A->columns, result);
  }
  if (!error) {
    error = cycle_mult_number_matrix(A, number, result);
  }
  return error;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = check_bad_matrix(A) || check_bad_matrix(B);
  if (!error) {
    if (A->columns != B->rows) {
      error = 2;
    }
  }
  if (!error) {
    error = s21_create_matrix(A->rows, A->rows, result);
  }
  if (!error) {
    error = cycle_mult_matrix(A, B, result);
  }
  return error;
}

static int check_eq_dim(matrix_t *A, matrix_t *B) {
  int error = 0;
  if ((A->rows != B->rows) || (A->columns != B->columns)) {
    error = 2;
  }
  return error;
}

static int cycle_eq_matrix(matrix_t *A, matrix_t *B) {
  int error = 0;
  for (int i = 0; i < A->rows && !error; i++) {
    if (!(A->matrix[i]) || !(B->matrix[i])) {
      error = 1;
    }
    for (int j = 0; j < A->columns && !error; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
        error = 1;
      }
    }
  }
  return error;
}

static int cycle_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = 0;
  for (int i = 0; i < A->rows && !error; i++) {
    if (!(A->matrix[i]) || !(B->matrix[i])) {
      error = 1;
    }
    for (int j = 0; j < A->columns && !error; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return error;
}

static int cycle_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = 0;
  for (int i = 0; i < A->rows && !error; i++) {
    if (!(A->matrix[i]) || !(B->matrix[i])) {
      error = 1;
    }
    for (int j = 0; j < A->columns && !error; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return error;
}

static int cycle_mult_number_matrix(matrix_t *A, double number,
                                    matrix_t *result) {
  int error = 0;
  for (int i = 0; i < A->rows && !error; i++) {
    if (!(A->matrix[i])) {
      error = 1;
    }
    for (int j = 0; j < A->columns && !error; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return error;
}

static int cycle_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int error = 0;
  for (int i = 0; i < A->rows; i++) {
    if (!(A->matrix[i])) {
      error = 1;
    }
  }
  for (int i = 0; i < B->rows; i++) {
    if (!(B->matrix[i])) {
      error = 1;
    }
  }
  for (int i = 0; i < A->rows && !error; i++) {
    for (int j = 0; j < B->columns; j++) {
      result->matrix[i][j] = calc_mul_elem(A, B, i, j);
    }
  }
  return error;
}

double calc_mul_elem(matrix_t *A, matrix_t *B, int i, int j) {
  double sum = 0;
  for (int k = 0; k < A->columns; k++) {
    sum += A->matrix[i][k] * B->matrix[k][j];
  }
  return sum;
}
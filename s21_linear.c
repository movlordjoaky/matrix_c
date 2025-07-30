#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_matrix.h"

static int cycle_transpose_matrix(matrix_t *A, matrix_t *result);
static int det_by_lu(matrix_t *A, double *det);
static int create_L(matrix_t *L, int n);
static int create_U(matrix_t *A, matrix_t *U, int n);
static int create_p(int **p, int n);
static int lu_decomposition(int n, matrix_t *L, matrix_t *U, int *p, int *s);
static void perm(matrix_t *U, int *p, int *s, int n, int r1, int r2);
static void remove_vector(int **p);
static int is_square(matrix_t *A);
static int create_minor(matrix_t *A, int n, int i, int j, matrix_t *minor);

int s21_transpose(matrix_t *A, matrix_t *result) {
  int error = check_bad_matrix(A);
  if (!error) {
    error = s21_create_matrix(A->columns, A->rows, result);
  }
  if (!error) {
    error = cycle_transpose_matrix(A, result);
  }
  return error;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int error = check_bad_matrix(A), n = A->rows;
  if (!error) {
    error = is_square(A) ? 0 : 2;
  }
  if (!error) {
    error = s21_create_matrix(A->columns, A->rows, result);
  }
  for (int i = 0; i < n && !error; i++) {
    for (int j = 0; j < n && !error; j++) {
      matrix_t minor;
      double det;
      error = create_minor(A, n, i, j, &minor);
      if (!error) {
        error = s21_determinant(&minor, &det);
        result->matrix[i][j] = det * ((i + j) & 1 ? -1 : 1);
      }
      s21_remove_matrix(&minor);
    }
  }
  return error;
}

static int create_minor(matrix_t *A, int n, int i, int j, matrix_t *minor) {
  int error = 0;
  error = s21_create_matrix(n - 1, n - 1, minor);
  if (!error) {
    for (int k = 0, k_m = 0; k < n; k++) {
      if (k != i) {
        for (int l = 0, l_m = 0; l < n; l++) {
          if (l != j) {
            minor->matrix[k_m][l_m] = A->matrix[k][l];
            l_m++;
          }
        }
        k_m++;
      }
    }
  }
  return error;
}

int s21_determinant(matrix_t *A, double *result) {
  int error = check_bad_matrix(A);
  if (!error) {
    error = is_square(A) ? 0 : 2;
  }
  if (!error) {
    error = det_by_lu(A, result);
  }
  return error;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  double det;
  int error = check_bad_matrix(A), n = A->rows;
  matrix_t L, U;
  int *p;
  if (!error) {
    error = is_square(A) ? 0 : 2;
  }
  if (!error) {
    error = create_L(&L, n) || create_U(A, &U, n) || create_p(&p, n);
  }
  if (!error) {
    error = det_by_lu(A, &det);
    error = det == 0 ? 2 : error;
  }
  if (!error) {
    matrix_t M, MT;
    error = s21_calc_complements(A, &M) || s21_transpose(&M, &MT) ||
            s21_mult_number(&MT, 1.0 / det, result);
    s21_remove_matrix(&M);
    s21_remove_matrix(&MT);
  }
  s21_remove_matrix(&L);
  s21_remove_matrix(&U);
  remove_vector(&p);
  return error;
}

static int cycle_transpose_matrix(matrix_t *A, matrix_t *result) {
  int error = 0;
  for (int i = 0; i < A->rows && !error; i++) {
    if (!(A->matrix[i])) {
      error = 1;
    }
    for (int j = 0; j < A->columns && !error; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return error;
}

static int det_by_lu(matrix_t *A, double *det) {
  matrix_t L, U;
  int n = A->rows, error = 0, s = 0;
  int *p;
  error = create_L(&L, n) || create_U(A, &U, n) || create_p(&p, n);
  if (!error) {
    int singular = 0;
    singular = lu_decomposition(n, &L, &U, p, &s);
    if (singular) {
      *det = 0;
    } else {
      *det = 1;
      for (int i = 0; i < n; i++) {
        *det *= U.matrix[i][i];
      }
      *det *= (s & 1 ? -1 : 1);
    }
  }
  s21_remove_matrix(&L);
  s21_remove_matrix(&U);
  remove_vector(&p);
  return error;
}

static int create_L(matrix_t *L, int n) {
  int error = 0;
  error = s21_create_matrix(n, n, L);
  if (!error) {
    for (int i = 0; i < n; i++) {
      L->matrix[i][i] = 1;
    }
  }
  return error;
}

static int create_U(matrix_t *A, matrix_t *U, int n) {
  int error = 0;
  error = s21_create_matrix(n, n, U);
  if (!error) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        U->matrix[i][j] = A->matrix[i][j];
      }
    }
  }
  return error;
}

static int create_p(int **p, int n) {
  int error = 0;
  *p = NULL;
  *p = malloc(n * sizeof(int));
  if (!*p) {
    error = 1;
  } else {
    for (int i = 0; i < n; i++) {
      (*p)[i] = i;
    }
  }
  return error;
}

static void remove_vector(int **p) { free(*p); }

static int lu_decomposition(int n, matrix_t *L, matrix_t *U, int *p, int *s) {
  int singular = 0;
  for (int k = 0; k < n - 1; k++) {
    int max_elem_i = k;
    for (int i = k + 1; i < n; i++) {
      if (fabs(U->matrix[i][k]) > fabs(U->matrix[max_elem_i][k])) {
        max_elem_i = i;
      }
    }
    if (max_elem_i != k) {
      perm(U, p, s, n, max_elem_i, k);
    }
    if (U->matrix[k][k] == 0) {
      singular = 1;
    }
    for (int i = k + 1; i < n && !singular; i++) {
      double factor = U->matrix[i][k] / U->matrix[k][k];
      L->matrix[i][k] = factor;
      for (int j = 0; j < k; j++) {
        U->matrix[i][j] = 0;
      }
      for (int j = k; j < n; j++) {
        U->matrix[i][j] -= U->matrix[k][j] * factor;
      }
    }
  }
  if (U->matrix[n - 1][n - 1] == 0) {
    singular = 1;
  }
  return singular;
}

static void perm(matrix_t *U, int *p, int *s, int n, int r1, int r2) {
  double temp_double;
  for (int j = 0; j < n; j++) {
    temp_double = U->matrix[r1][j];
    U->matrix[r1][j] = U->matrix[r2][j];
    U->matrix[r2][j] = temp_double;
  }
  int temp_int = p[r1];
  p[r1] = p[r2];
  p[r2] = temp_int;
  (*s)++;
}

static int is_square(matrix_t *A) {
  int result = 0;
  if (A->columns == A->rows) {
    result = 1;
  }
  return result;
}
#include "s21_matrix.h"

#include <stdlib.h>

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = 0;
  double *data = NULL;
  result->matrix = NULL;
  if (rows <= 0 || columns <= 0) {
    error = 1;
  }
  if (!error) {
    result->matrix = malloc(rows * sizeof(double *));
    if (!(result->matrix)) {
      error = 1;
    }
  }
  if (!error) {
    data = calloc(rows * columns, sizeof(double));
    if (!data) {
      error = 1;
      free(result->matrix);
      result->matrix = NULL;
    }
  }
  if (!error) {
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = data + i * columns;
    }
    result->rows = rows;
    result->columns = columns;
  }
  return error;
}

void s21_remove_matrix(matrix_t *A) {
  if (A && A->matrix) {
    if (A->matrix[0]) {
      free(A->matrix[0]);
      A->matrix[0] = NULL;
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

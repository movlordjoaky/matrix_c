#include "s21_matrix.h"

int check_bad_matrix(matrix_t *A) {
  int error = 0;
  if (!A || !(A->matrix) || A->columns <= 0 || A->rows <= 0) {
    error = 1;
  } else {
    for (int i = 0; i < A->rows && !error; i++) {
      if (!(A->matrix[i])) {
        error = 1;
      }
    }
  }
  return error;
}

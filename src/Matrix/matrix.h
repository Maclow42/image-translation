#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct matrix
{
    size_t row;
    size_t col;
    float *data;
} matrix;

// Matrix creation and destruction
matrix* matrix_create(const size_t row, const size_t col, float value);
matrix* matrix_create_from_function(const size_t row, const size_t col, float (*f)(size_t, size_t));
matrix* matrix_create_random(const size_t row, const size_t col, float lower, float upper);
void matrix_destroy(matrix *m);

// Matrix setter and getter
float matrix_get(const matrix *m, const size_t row, const size_t col);
float matrix_set(matrix *m, const size_t row, const size_t col, float val);

// Matrix operations
matrix* matrix_add(const matrix *m1, const matrix *m2);
void matrix_add_inplace(matrix *dest, const matrix *src);

matrix* matrix_sub(const matrix *m1, const matrix *m2);
void matrix_sub_inplace(matrix *dest, const matrix *src);

matrix* matrix_mul(const matrix *m1, const matrix *m2);

matrix* matrix_scalar_add(const matrix *m, float scalar);
void matrix_scalar_add_inplace(matrix *m, float scalar);

matrix* matrix_scalar_sub(const matrix *m, float scalar);
void matrix_scalar_sub_inplace(matrix *m, float scalar);

matrix* matrix_scalar_mul(const matrix *m, float scalar);
void matrix_scalar_mul_inplace(matrix *m, float scalar);

matrix* matrix_scalar_div(const matrix *m, float scalar);
void matrix_scalar_div_inplace(matrix *m, float scalar);

matrix* matrix_dot(const matrix *m1, const matrix *m2);
void matrix_dot_inplace(matrix *dest, const matrix *src);

matrix* matrix_transpose(const matrix *m);

void matrix_apply(const matrix *m, float (*f)(float));

// Matrix comparison
bool matrix_equals(const matrix *m1, const matrix *m2);

// Raw and column operations
matrix* matrix_get_row(const matrix *m, const size_t row);
matrix* matrix_get_col(const matrix *m, const size_t col);

void matrix_set_row(matrix *m, const float *row, const size_t row_index);
void matrix_set_col(matrix *m, const float *col, const size_t col_index);

// Matrix utility functions
void matrix_print(const matrix *m);
void matrix_fill(matrix *m, float value);

void matrix_copy_to(const matrix *src, matrix *dest);
matrix* matrix_get_copy(const matrix *m);

matrix matrix_resize(const matrix *m, const size_t row, const size_t col);

// Special Matrices
matrix* matrix_identity(const size_t size);
matrix* matrix_zeros(const size_t row, const size_t col);
matrix* matrix_ones(const size_t row, const size_t col);
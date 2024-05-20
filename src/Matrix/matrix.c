#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Matrix creation and destruction
matrix* matrix_create(const size_t row, const size_t col, float value){
    matrix *m = matrix_zeros(row, col);

    if(m == NULL){
        fprintf(stderr, "matrix_create: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < row * col; i++){
        m->data[i] = value;
    }
    return m;
}

matrix* matrix_create_from_function(const size_t row, const size_t col, float (*f)(size_t, size_t)){
    matrix *m = matrix_zeros(row, col);

    if(m == NULL){
        fprintf(stderr, "matrix_create_from_function: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < row; i++){
        for(size_t j = 0; j < col; j++){
            m->data[i * col + j] = f(i, j);
        }
    }
    return m;
}

matrix* matrix_create_random(const size_t row, const size_t col, float lower, float upper){
    matrix *m = matrix_zeros(row, col);

    if(m == NULL){
        fprintf(stderr, "matrix_create_random: Failed to allocate memory for matrix\n");
        return NULL;
    }

    srand(time(NULL));
    for(size_t i = 0; i < row * col; i++){
        m->data[i] = (float)rand() / RAND_MAX * (upper - lower) + lower;
    }
    return m;
}

void matrix_destroy(matrix *m){
    free(m->data);
    free(m);
}

// Matrix setter and getter
float matrix_get(const matrix *m, const size_t row, const size_t col){
    if(row >= m->row || col >= m->col){
        fprintf(stderr, "matrix_get: Index out of bounds\n");
        return 0;
    }

    return m->data[row * m->col + col];
}

float matrix_set(matrix *m, const size_t row, const size_t col, float val){
    if(row >= m->row || col >= m->col){
        fprintf(stderr, "matrix_set: Index out of bounds\n");
        return 0;
    }

    m->data[row * m->col + col] = val;
    return val;
}


// Matrix operations
matrix* matrix_add(const matrix *m1, const matrix *m2){
    if(m1->row != m2->row || m1->col != m2->col){
        fprintf(stderr, "matrix_add: Matrix dimensions do not match\n");
        return NULL;
    }

    matrix *m = matrix_zeros(m1->row, m1->col);

    if(m == NULL){
        fprintf(stderr, "matrix_add: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < m1->row * m1->col; i++){
        m->data[i] = m1->data[i] + m2->data[i];
    }
    return m;
}

void matrix_add_inplace(matrix *dest, const matrix *src){
    if(dest->row != src->row || dest->col != src->col){
        fprintf(stderr, "matrix_add_inplace: Matrix dimensions do not match\n");
        return;
    }

    for(size_t i = 0; i < dest->row * dest->col; i++){
        dest->data[i] += src->data[i];
    }
}

matrix* matrix_sub(const matrix *m1, const matrix *m2){
    if(m1->row != m2->row || m1->col != m2->col){
        fprintf(stderr, "matrix_sub: Matrix dimensions do not match\n");
        return NULL;
    }

    matrix *m = matrix_zeros(m1->row, m1->col);

    if(m == NULL){
        fprintf(stderr, "matrix_sub: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < m1->row * m1->col; i++){
        m->data[i] = m1->data[i] - m2->data[i];
    }
    return m;
}

void matrix_sub_inplace(matrix *dest, const matrix *src){
    if(dest->row != src->row || dest->col != src->col){
        fprintf(stderr, "matrix_sub_inplace: Matrix dimensions do not match\n");
        return;
    }

    for(size_t i = 0; i < dest->row * dest->col; i++){
        dest->data[i] -= src->data[i];
    }
}

matrix* matrix_mul(const matrix *m1, const matrix *m2){
    if(m1->col != m2->row){
        fprintf(stderr, "matrix_mul: Matrix dimensions do not match\n");
        return NULL;
    }

    matrix *m = matrix_zeros(m1->row, m2->col);

    if(m == NULL){
        fprintf(stderr, "matrix_mul: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < m1->row; i++){
        for(size_t j = 0; j < m2->col; j++){
            for(size_t k = 0; k < m1->col; k++){
                m->data[i * m->col + j] += m1->data[i * m1->col + k] * m2->data[k * m2->col + j];
            }
        }
    }
    return m;
}

matrix* matrix_scalar_add(const matrix *m, float scalar){
    matrix *res = matrix_get_copy(m);

    if(res == NULL){
        fprintf(stderr, "matrix_scalar_add: Failed to allocate memory for matrix\n");
        return NULL;
    }
    
    matrix_scalar_add_inplace(res, scalar);
    return res;
}

void matrix_scalar_add_inplace(matrix *m, float scalar){
    for(size_t i = 0; i < m->row * m->col; i++){
        m->data[i] += scalar;
    }
}

matrix* matrix_scalar_sub(const matrix *m, float scalar){
    matrix *res = matrix_get_copy(m);

    if(res == NULL){
        fprintf(stderr, "matrix_scalar_sub: Failed to allocate memory for matrix\n");
        return NULL;
    }

    matrix_scalar_sub_inplace(res, scalar);
    return res;
}

void matrix_scalar_sub_inplace(matrix *m, float scalar){
    for(size_t i = 0; i < m->row * m->col; i++){
        m->data[i] -= scalar;
    }
}

matrix* matrix_scalar_mul(const matrix *m, float scalar){
    matrix *res = matrix_get_copy(m);

    if(res == NULL){
        fprintf(stderr, "matrix_scalar_mul: Failed to allocate memory for matrix\n");
        return NULL;
    }

    matrix_scalar_mul_inplace(res, scalar);
    return res;
}

void matrix_scalar_mul_inplace(matrix *m, float scalar){
    for(size_t i = 0; i < m->row * m->col; i++){
        m->data[i] *= scalar;
    }
}

matrix* matrix_scalar_div(const matrix *m, float scalar){
    matrix *res = matrix_get_copy(m);

    if(res == NULL){
        fprintf(stderr, "matrix_scalar_div: Failed to allocate memory for matrix\n");
        return NULL;
    }

    matrix_scalar_div_inplace(res, scalar);
    return res;
}

void matrix_scalar_div_inplace(matrix *m, float scalar){
    for(size_t i = 0; i < m->row * m->col; i++){
        m->data[i] /= scalar;
    }
}

matrix* matrix_dot(const matrix *m1, const matrix *m2){
    matrix *m = matrix_zeros(m1->row, m2->col);

    if(m == NULL){
        fprintf(stderr, "matrix_dot: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < m1->row; i++){
        for(size_t j = 0; j < m2->col; j++){
            m->data[i * m->col + j] = m1->data[i * m1->col + j] * m2->data[i * m2->col + j];
        }
    }
    return m;
}

void matrix_dot_inplace(matrix *dest, const matrix *src){
    // element by element multiplication
    for(size_t i = 0; i < dest->row * dest->col; i++){
        dest->data[i] *= src->data[i];
    }
}

matrix* matrix_transpose(const matrix *m){
    matrix *res = matrix_zeros(m->col, m->row);

    if(res == NULL){
        fprintf(stderr, "matrix_transpose: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < m->row; i++){
        for(size_t j = 0; j < m->col; j++){
            res->data[j * res->col + i] = m->data[i * m->col + j];
        }
    }
    return res;
}

void matrix_apply(const matrix *m, float (*f)(float)){
    for(size_t i = 0; i < m->row * m->col; i++){
        m->data[i] = f(m->data[i]);
    }
}

// Matrix comparison
bool matrix_equals(const matrix *m1, const matrix *m2){
    if(m1->row != m2->row || m1->col != m2->col){
        return false;
    }
    for(size_t i = 0; i < m1->row * m1->col; i++){
        if(m1->data[i] != m2->data[i]){
            return false;
        }
    }
    return true;
}

// Raw and column operations
matrix* matrix_get_row(const matrix *m, const size_t row){
    matrix *res = matrix_zeros(1, m->col);

    if(res == NULL){
        fprintf(stderr, "matrix_get_row: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < m->col; i++){
        res->data[i] = m->data[row * m->col + i];
    }
    return res;
}

matrix* matrix_get_col(const matrix *m, const size_t col){
    matrix *res = matrix_zeros(m->row, 1);

    if(res == NULL){
        fprintf(stderr, "matrix_get_col: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < m->row; i++){
        res->data[i] = m->data[i * m->col + col];
    }
    return res;
}

void matrix_set_row(matrix *m, const float *row, const size_t row_index){
    for(size_t i = 0; i < m->col; i++){
        m->data[row_index * m->col + i] = row[i];
    }
}

void matrix_set_col(matrix *m, const float *col, const size_t col_index){
    for(size_t i = 0; i < m->row; i++){
        m->data[i * m->col + col_index] = col[i];
    }
}

// Matrix utility functions
void matrix_print(const matrix *m){
    for(size_t i = 0; i < m->row; i++){
        for(size_t j = 0; j < m->col; j++){
            printf("%f ", m->data[i * m->col + j]);
        }
        printf("\n");
    }
}

void matrix_fill(matrix *m, float value){
    for(size_t i = 0; i < m->row * m->col; i++)
        m->data[i] = value;
}

void matrix_copy_to(const matrix *src, matrix *dest){
    if(src->row != dest->row || src->col != dest->col){
        fprintf(stderr, "matrix_copy_to: Matrix dimensions do not match\n");
        return;
    }
    for(size_t i = 0; i < src->row * src->col; i++){
        dest->data[i] = src->data[i];
    }
}

matrix* matrix_get_copy(const matrix *m){
    matrix *res = matrix_zeros(m->row, m->col);

    if(res == NULL){
        fprintf(stderr, "matrix_get_copy: Failed to allocate memory for matrix\n");
        return NULL;
    }

    matrix_copy_to(m, res);
    return res;
}

matrix matrix_resize(const matrix *m, const size_t row, const size_t col){
    matrix res;
    res.row = row;
    res.col = col;
    res.data = calloc(row * col, sizeof(float));
    for(size_t i = 0; i < row; i++){
        for(size_t j = 0; j < col; j++){
            if(i < m->row && j < m->col){
                res.data[i * col + j] = m->data[i * m->col + j];
            }
        }
    }
    return res;
}

// Special Matrices
matrix* matrix_identity(const size_t size){
    matrix *m = matrix_zeros(size, size);

    if(m == NULL){
        fprintf(stderr, "matrix_identity: Failed to allocate memory for matrix\n");
        return NULL;
    }

    for(size_t i = 0; i < size; i++){
        m->data[i * size + i] = 1;
    }
    return m;
}

matrix* matrix_zeros(const size_t row, const size_t col){
    matrix *m = malloc(sizeof(matrix));

    if(m == NULL){
        fprintf(stderr, "matrix_zeros: Failed to allocate memory for matrix\n");
        return NULL;
    }

    m->row = row;
    m->col = col;
    m->data = calloc(row * col, sizeof(float));
    return m;
}

matrix* matrix_ones(const size_t row, const size_t col){
    matrix *m = matrix_zeros(row, col);

    if(m == NULL){
        fprintf(stderr, "matrix_ones: Failed to allocate memory for matrix\n");
        return NULL;
    }

    matrix_fill(m, 1);
    return m;
}
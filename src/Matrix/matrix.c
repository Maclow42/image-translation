#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Matrix creation and destruction
matrix matrix_create(const size_t row, const size_t col, float value){
    matrix m = {
        .row = row,
        .col = col,
        .data = (float *) calloc(row * col, sizeof(float))};
    matrix_fill(&m, value);
    return m;
}

matrix matrix_create_from_function(const size_t row, const size_t col, float (*f)(size_t, size_t)){
    matrix m = {
        .row = row,
        .col = col,
        .data = (float *) calloc(row * col, sizeof(float))};
    for (size_t i = 0; i < row; i++){
        for (size_t j = 0; j < col; j++){
            matrix_set(&m, i, j, f(i, j));
        }
    }
    return m;
}

matrix matrix_create_random(const size_t row, const size_t col, float lower, float upper){
    matrix m = {
        .row = row,
        .col = col,
        .data = (float *) calloc(row * col, sizeof(float))};
    for (size_t i = 0; i < row; i++){
        for (size_t j = 0; j < col; j++){
            matrix_set(&m, i, j, (float) rand() / RAND_MAX * (upper - lower) + lower);
        }
    }
    return m;
}

void matrix_destroy(matrix *m){
    free(m->data);
    m->data = NULL;
    m->row = 0;
    m->col = 0;
}

// Matrix setter and getter
float matrix_get(const matrix *m, const size_t row, const size_t col){
    if(row >= m->row || col >= m->col){
        printf("matrix_get : Index out of bounds\n");
        exit(1);
    }

    return m->data[row * m->col + col];
}

float matrix_set(matrix *m, const size_t row, const size_t col, float val){
    if(row >= m->row || col >= m->col){
        printf("matrix_set : Index out of bounds\n");
        exit(1);
    }

    m->data[row * m->col + col] = val;
    return val;
}

// Matrix operations
matrix matrix_add(const matrix *m1, const matrix *m2){
    if(m1->row != m2->row || m1->col != m2->col){
        printf("matrix_add : Matrix dimensions do not match\n");
        exit(1);
    }

    matrix result = matrix_create(m1->row, m1->col, 0);
    for (size_t i = 0; i < m1->row * m1->col; i++)
        result.data[i] = m1->data[i] + m2->data[i];
    
    return result;
}
matrix matrix_sub(const matrix *m1, const matrix *m2){
    if(m1->row != m2->row || m1->col != m2->col){
        printf("matrix_sub : Matrix dimensions do not match\n");
        exit(1);
    }

    matrix result = matrix_create(m1->row, m1->col, 0);
    for (size_t i = 0; i < m1->row * m1->col; i++)
        result.data[i] = m1->data[i] - m2->data[i];

    return result;
}

matrix matrix_mul(const matrix *m1, const matrix *m2){
    if(m1->col != m2->row){
        printf("matrix_mul : Matrix dimensions do not match\n");
        exit(1);
    }

    matrix result = matrix_create(m1->row, m2->col, 0);
    for (size_t i = 0; i < m1->row; i++){
        for (size_t j = 0; j < m2->col; j++){
            float sum = 0;
            for (size_t k = 0; k < m1->col; k++)
                sum += matrix_get(m1, i, k) * matrix_get(m2, k, j);
            matrix_set(&result, i, j, sum);
        }
    }
    return result;
}

matrix matrix_scalar_add(const matrix *m, float scalar){
    matrix result = matrix_create(m->row, m->col, 0);
    for (size_t i = 0; i < m->row * m->col; i++)
        result.data[i] = m->data[i] + scalar;

    return result;
}

matrix matrix_scalar_sub(const matrix *m, float scalar){
    return matrix_scalar_add(m, -scalar);
}

matrix matrix_scalar_mul(const matrix *m, float scalar){
    matrix result = matrix_create(m->row, m->col, 0);
    for (size_t i = 0; i < m->row * m->col; i++)
        result.data[i] = m->data[i] * scalar;

    return result;
}

matrix matrix_scalar_div(const matrix *m, float scalar){
    return matrix_scalar_mul(m, 1 / scalar);
}

matrix matrix_dot(const matrix *m1, const matrix *m2){
    if(m1->row != m2->row || m1->col != m2->col){
        printf("matrix_dot : Matrix dimensions do not match\n");
        exit(1);
    }

    matrix result = matrix_create(m1->row, m1->col, 0);
    for (size_t i = 0; i < m1->row * m1->col; i++)
        result.data[i] = m1->data[i] * m2->data[i];

    return result;
}

matrix matrix_transpose(const matrix *m){
    matrix result = matrix_create(m->col, m->row, 0);
    for (size_t i = 0; i < m->row; i++){
        for (size_t j = 0; j < m->col; j++){
            matrix_set(&result, j, i, matrix_get(m, i, j));
        }
    }
    return result;
}

matrix matrix_apply(const matrix *m, float (*f)(float)){
    matrix result = matrix_create(m->row, m->col, 0);
    for (size_t i = 0; i < m->row * m->col; i++)
        result.data[i] = f(m->data[i]);

    return result;
}

void matrix_apply_inplace(matrix *m, float (*f)(float)){
    for (size_t i = 0; i < m->row * m->col; i++)
        m->data[i] = f(m->data[i]);
}

// Matrix comparison
bool matrix_equals(const matrix *m1, const matrix *m2){
    if(m1->row != m2->row || m1->col != m2->col)
        return false;

    for (size_t i = 0; i < m1->row * m1->col; i++)
        if(m1->data[i] != m2->data[i])
            return false;

    return true;
}

// Raw and column operations
matrix matrix_get_row(const matrix *m, const size_t row){
    if(row >= m->row){
        printf("matrix_get_row : Index out of bounds\n");
        exit(1);
    }

    matrix result = matrix_create(1, m->col, 0);
    for (size_t i = 0; i < m->col; i++)
        matrix_set(&result, 0, i, matrix_get(m, row, i));

    return result;
}
matrix matrix_get_col(const matrix *m, const size_t col){
    if(col >= m->col){
        printf("matrix_get_col : Index out of bounds\n");
        exit(1);
    }

    matrix result = matrix_create(m->row, 1, 0);
    for (size_t i = 0; i < m->row; i++)
        matrix_set(&result, i, 0, matrix_get(m, i, col));

    return result;
}

void matrix_set_row(matrix *m, const matrix *row, const size_t row_index){
    if(row_index >= m->row){
        printf("matrix_set_row : Index out of bounds\n");
        exit(1);
    }

    for (size_t i = 0; i < m->col; i++)
        matrix_set(m, row_index, i, matrix_get(row, 0, i));
}

void matrix_set_col(matrix *m, const matrix *col, const size_t col_index){
    if(col_index >= m->col){
        printf("matrix_set_col : Index out of bounds\n");
        exit(1);
    }

    for (size_t i = 0; i < m->row; i++)
        matrix_set(m, i, col_index, matrix_get(col, i, 0));
}

matrix matrix_remove_row(const matrix *m, const size_t row_index){
    if(row_index >= m->row){
        printf("matrix_remove_row : Index out of bounds\n");
        exit(1);
    }

    matrix result = matrix_create(m->row - 1, m->col, 0);
    for (size_t i = 0; i < m->row; i++){
        if(i < row_index){
            for (size_t j = 0; j < m->col; j++)
                matrix_set(&result, i, j, matrix_get(m, i, j));
        }
        else if(i > row_index){
            for (size_t j = 0; j < m->col; j++)
                matrix_set(&result, i - 1, j, matrix_get(m, i, j));
        }
    }

    return result;
}
matrix matrix_remove_col(const matrix *m, const size_t col_index){
    if(col_index >= m->col){
        printf("matrix_remove_col : Index out of bounds\n");
        exit(1);
    }

    matrix result = matrix_create(m->row, m->col - 1, 0);
    for (size_t i = 0; i < m->row; i++){
        for (size_t j = 0; j < m->col; j++){
            if(j < col_index)
                matrix_set(&result, i, j, matrix_get(m, i, j));
            else if(j > col_index)
                matrix_set(&result, i, j - 1, matrix_get(m, i, j));
        }
    }

    return result;
}

matrix matrix_insert_row(const matrix *m, const matrix *row, const size_t row_index){
    if(row_index > m->row){
        printf("matrix_insert_row : Index out of bounds\n");
        exit(1);
    }

    matrix result = matrix_create(m->row + 1, m->col, 0);
    for (size_t i = 0; i < m->row; i++){
        if(i < row_index){
            for (size_t j = 0; j < m->col; j++)
                matrix_set(&result, i, j, matrix_get(m, i, j));
        }
        else{
            for (size_t j = 0; j < m->col; j++)
                matrix_set(&result, i + 1, j, matrix_get(m, i, j));
        }
    }

    for (size_t i = 0; i < m->col; i++)
        matrix_set(&result, row_index, i, matrix_get(row, 0, i));

    return result;
}

matrix matrix_insert_col(const matrix *m, const matrix *col, const size_t col_index){
    if(col_index > m->col){
        printf("matrix_insert_col : Index out of bounds\n");
        exit(1);
    }

    matrix result = matrix_create(m->row, m->col + 1, 0);
    for (size_t i = 0; i < m->row; i++){
        for (size_t j = 0; j < m->col; j++){
            if(j < col_index)
                matrix_set(&result, i, j, matrix_get(m, i, j));
            else
                matrix_set(&result, i, j + 1, matrix_get(m, i, j));
        }
    }

    for (size_t i = 0; i < m->row; i++)
        matrix_set(&result, i, col_index, matrix_get(col, i, 0));

    return result;
}

// Matrix utility functions
void matrix_print(const matrix *m){
    for (size_t i = 0; i < m->row; i++){
        for (size_t j = 0; j < m->col; j++){
            printf("%f ", matrix_get(m, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void matrix_fill(matrix *m, float value){
    for (size_t i = 0; i < m->row; i++){
        for (size_t j = 0; j < m->col; j++){
            matrix_set(m, i, j, value);
        }
    }
}

void matrix_copy_to(const matrix *src, matrix *dest){
    if(src->row != dest->row || src->col != dest->col){
        printf("matrix_copy_to : Matrix dimensions do not match\n");
        exit(1);
    }

    for (size_t i = 0; i < src->row * src->col; i++)
        dest->data[i] = src->data[i];
}

matrix matrix_get_copy(const matrix *m){
    matrix result = matrix_create(m->row, m->col, 0);
    matrix_copy_to(m, &result);
    return result;
}

matrix matrix_resize(const matrix *m, const size_t row, const size_t col){
    matrix result = matrix_create(row, col, 0);
    for (size_t i = 0; i < m->row && i < row; i++){
        for (size_t j = 0; j < m->col && j < col; j++){
            matrix_set(&result, i, j, matrix_get(m, i, j));
        }
    }
    return result;
}

// Special Matrices
matrix matrix_identity(const size_t size){
    matrix m = matrix_create(size, size, 0);
    for (size_t i = 0; i < size; i++)
        matrix_set(&m, i, i, 1);
    return m;
}

matrix matrix_zeros(const size_t row, const size_t col){
    return matrix_create(row, col, 0);
}

matrix matrix_ones(const size_t row, const size_t col){
    return matrix_create(row, col, 1);
}
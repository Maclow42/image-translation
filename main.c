#include "src/Matrix/matrix.h"
#include <stdio.h>

int main(){
    matrix m1 = matrix_create_random(3, 3, 0, 10);
    matrix m2 = matrix_create_random(3, 3, 0, 10);

    matrix_print(&m1);
    matrix_print(&m2);

    matrix m3 = matrix_add(&m1, &m2);
    matrix_print(&m3);

    matrix m4 = matrix_get_copy(&m1);
    if(matrix_equals(&m1, &m4)){
        printf("m1 and m4 are equal\n");
    }else{
        printf("m1 and m4 are not equal\n");
    }

    return 0;
}
#include <stdio.h>
#include <math.h>

#include "src/Matrix/matrix.h"
#include "src/NeuralNetwork/neuralNetwork.h"

float sigmoid(float x){
    return 1 / (1 + expf(-x));
}

float sigmoid_prime(float x){
    return x * (1 - x);
}

float relu(float x){
    return x > 0 ? x : 0;
}

float relu_prime(float x){
    return x > 0 ? 1 : 0;
}

int main(){
    matrix* a = matrix_create(1, 3, 1);
    matrix_set_row(a, (float[]){1, 3, 1}, 0);

    matrix* b = matrix_create(3, 2, 1);
    matrix_set_row(b, (float[]){1, 2}, 0);
    matrix_set_row(b, (float[]){3, 4}, 1);
    matrix_set_row(b, (float[]){1, 2}, 2);

    matrix* c = matrix_mul(a, b);
    matrix_print(c);


    // XOR dataset
    matrix *X = matrix_create(2, 4, 0);
    matrix_set_row(X, (float[]){0, 1, 0, 1}, 0);
    matrix_set_row(X, (float[]){0, 0, 1, 1}, 1);

    matrix *y = matrix_create(1, 4, 0);
    matrix_set_row(y, (float[]){0, 1, 1, 0}, 0);

    neural_network *nn = neural_network_create();
    nn_set_learning_rate(nn, 0.1);
    nn_set_loss_function(nn, MEAN_SQUARED_ERROR);
    nn_set_input_layer(nn, 5, sigmoid, sigmoid_prime);
    nn_set_output_layer(nn, 1, sigmoid, sigmoid_prime);

    //nn_train(nn, X, y, 1);

    return 0;
}
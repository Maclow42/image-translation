#include <stdlib.h>
#include <stdio.h>

#include "../Matrix/matrix.h"
#include "../list/list.h"

typedef enum layer_type{
    INPUT,
    HIDDEN,
    OUTPUT
} layer_type;

typedef enum loss_function{
    MEAN_SQUARED_ERROR,
    CROSS_ENTROPY
} loss_function;

typedef struct layer{
    layer_type type;
	size_t input_size;
    size_t nb_neurons;
	matrix *weights;
    float (*activation)(float);
    float (*activation_prime)(float);
} layer;

typedef struct neural_network{
    layer **layers;
    size_t nb_layers;
    size_t max_layers;
    loss_function loss_function;
    float learning_rate;
    float dropout_rate;
    float momentum_rate;
    size_t batch_size;
} neural_network;

// Layer creation and destruction
layer* layer_create(layer_type type, size_t nb_neurons, size_t input_size, float (*activation)(float), float (*activation_prime)(float));
void layer_destroy(layer *l);

// Neural network creation and destruction
neural_network *neural_network_create();
void nn_destroy(neural_network *nn);

// Neural network parameters
void nn_set_input_layer(neural_network *nn, size_t nb_neurons, float (*activation)(float), float (*activation_prime)(float));
void nn_set_output_layer(neural_network *nn, size_t nb_neurons, float (*activation)(float), float (*activation_prime)(float));
void nn_add_hidden_layer(neural_network *nn, size_t nb_neurons, float (*activation)(float), float (*activation_prime)(float));
void nn_set_loss_function(neural_network *nn, loss_function loss_function);
void nn_set_learning_rate(neural_network *nn, float learning_rate);
void nn_set_dropout_rate(neural_network *nn, float dropout_rate);
void nn_set_momentum_rate(neural_network *nn, float momentum_rate);
void nn_set_batch_size(neural_network *nn, size_t batch_size);

// Neural network training
void nn_train(neural_network *nn, matrix *X, matrix *y, size_t epochs);
matrix *nn_predict(neural_network *nn, matrix *X);

// Neural network display
void nn_display_layers(neural_network *nn);
/**
 * @file neuralNetwork.c
 * @brief Implementation of the neural network functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "neuralNetwork.h"
#include "../Matrix/matrix.h"
#include "../list/list.h"

// Layer creation and destruction

/**
 * @brief Creates a layer with the specified number of neurons and activation functions.
 * 
 * @param nb_neurons The number of neurons in the layer.
 * @param activation The activation function for the layer.
 * @param activation_prime The derivative of the activation function for the layer.
 * @return The created layer.
 */
layer* layer_create(layer_type type, size_t nb_neurons, size_t input_size, float (*activation)(float), float (*activation_prime)(float)){
	layer *l = malloc(sizeof(layer));
	if(l == NULL){
		fprintf(stderr, "layer_create: Unable to allocate memory for the layer\n");
		exit(1);
	}
    l->type = type;
	l->input_size = input_size;
	l->nb_neurons = nb_neurons;
	l->activation = activation;
	l->activation_prime = activation_prime;
	return l;
}

/**
 * @brief Destroys a layer and frees the memory allocated for its weights->
 * 
 * @param l The layer to destroy.
 */
void layer_destroy(layer *l){
	matrix_destroy(l->weights);
	free(l);
}

// Neural network creation and destruction

/**
 * @brief Creates a neural network with the specified input size, output size, and loss function.
 * 
 * @return The created neural network.
 */
neural_network* neural_network_create(){
	neural_network *nn = malloc(sizeof(neural_network));

    if(nn == NULL){
        fprintf(stderr, "neural_network_create: Unable to allocate memory for the neural network\n");
        exit(1);
    }

    nn->layers = malloc(5 * sizeof(layer*));
    nn->nb_layers = 0;
    nn->max_layers = 5;
    nn->loss_function = MEAN_SQUARED_ERROR;
    nn->learning_rate = 0.01;
    nn->dropout_rate = 0;
    nn->momentum_rate = 0;
    nn->batch_size = 1;
	return nn;
}

/**
 * @brief Destroys a neural network and frees the memory allocated for its layers.
 * 
 * @param nn The neural network to destroy.
 */
void nn_destroy(neural_network *nn){
	for(size_t i = 0; i < nn->nb_layers; i++){
        layer_destroy(nn->layers[i]);
    }
}

// Neural network parameters

/**
 * @brief Sets the input layer of the neural network with the specified number of neurons and activation functions.
 * 
 * @param nn The neural network.
 * @param nb_neurons The number of neurons in the input layer.
 * @param activation The activation function for the input layer.
 * @param activation_prime The derivative of the activation function for the input layer.
 */
void nn_set_input_layer(neural_network *nn, size_t nb_neurons, float (*activation)(float), float (*activation_prime)(float)){
	if(nn->nb_layers > 0){
		fprintf(stderr, "nn_set_input_layer: Input layer already setted\n");
		exit(1);
	}

	// create the input layer
	// the input size is not setted yet (it will be setted in the nn_compile function)
	layer *l = layer_create(INPUT, nb_neurons, 0, activation, activation_prime);

    // add the input layer to the list
    // no size check because the list is initialized with 5 elements
    nn->layers[nn->nb_layers++] = l;
}

/**
 * @brief Sets the output layer of the neural network with the specified number of neurons and activation functions.
 * 
 * @param nn The neural network.
 * @param nb_neurons The number of neurons in the output layer.
 * @param activation The activation function for the output layer.
 * @param activation_prime The derivative of the activation function for the output layer.
 */
void nn_set_output_layer(neural_network *nn, size_t nb_neurons, float (*activation)(float), float (*activation_prime)(float)){
	if(nn->layers[nn->nb_layers - 1]->type == OUTPUT){
		fprintf(stderr, "nn_set_output_layer: Output layer already setted\n");
		exit(1);
	}

	if(nn->nb_layers == 0){
		fprintf(stderr, "nn_set_output_layer: Set the input layer first\n");
		exit(1);
	}

	// get the last added layer
	layer *last = nn->layers[nn->nb_layers - 1];

	// create the output layer
	layer *l = layer_create(OUTPUT, nb_neurons, last->nb_neurons, activation, activation_prime);

	// add the output layer to the list
    // if the list is full, we double its size
    if(nn->nb_layers == nn->max_layers)
        nn->layers = malloc(2*nn->nb_layers * sizeof(layer*));
    nn->layers[nn->nb_layers++] = l;
}

/**
 * @brief Adds a hidden layer to the neural network with the specified number of neurons and activation functions.
 * 
 * @param nn The neural network.
 * @param nb_neurons The number of neurons in the hidden layer.
 * @param activation The activation function for the hidden layer.
 * @param activation_prime The derivative of the activation function for the hidden layer.
 */
void nn_add_hidden_layer(neural_network *nn, size_t nb_neurons, float (*activation)(float), float (*activation_prime)(float)){
    if(nn->nb_layers == 0){
        fprintf(stderr, "nn_add_hidden_layer: Set the input layer first\n");
        exit(1);
    }

	if(nn->layers[nn->nb_layers - 1]->type == OUTPUT){
		fprintf(stderr, "nn_add_hidden_layer: Impossible to add a hidden layer after the output layer\n");
		exit(1);
	}

	// get the last added layer
	// exist because the input layer is already setted
	layer *last = nn->layers[nn->nb_layers - 1];

	// create the hidden layer
	layer *l = layer_create(HIDDEN, nb_neurons, last->nb_neurons, activation, activation_prime);

	// add the output layer to the list
    // if the list is full, we double its size
    if(nn->nb_layers == nn->max_layers)
        nn->layers = malloc(2*nn->nb_layers * sizeof(layer*));

    nn->layers[nn->nb_layers++] = l;
}

/**
 * @brief Sets the loss function of the neural network.
 * 
 * @param nn The neural network.
 * @param loss_function The loss function to be used for training the network.
 */
void nn_set_loss_function(neural_network *nn, loss_function loss_function){
	nn->loss_function = loss_function;
}

/**
 * @brief Sets the learning rate of the neural network.
 * 
 * @param nn The neural network.
 * @param learning_rate The learning rate to be used for training the network.
 */
void nn_set_learning_rate(neural_network *nn, float learning_rate){
	nn->learning_rate = learning_rate;
}

/**
 * @brief Sets the dropout rate of the neural network.
 * 
 * @param nn The neural network.
 * @param dropout_rate The dropout rate to be used for training the network.
 */
void nn_set_dropout_rate(neural_network *nn, float dropout_rate){
	nn->dropout_rate = dropout_rate;
}

/**
 * @brief Sets the momentum rate of the neural network.
 * 
 * @param nn The neural network.
 * @param momentum_rate The momentum rate to be used for training the network.
 */
void nn_set_momentum_rate(neural_network *nn, float momentum_rate){
	nn->momentum_rate = momentum_rate;
}

/**
 * @brief Sets the batch size of the neural network.
 * 
 * @param nn The neural network.
 * @param batch_size The batch size to be used for training the network.
 */
void nn_set_batch_size(neural_network *nn, size_t batch_size){
	nn->batch_size = batch_size;
}

// Neural network training

/**
 * @brief Compiles the layers of the neural network by initializing the weights matrices.
 * 
 * The weights are initialized with random values from -1 to 1.
 * The input layer is initialized with the input_layer_size x next_layer_size x.
 * The output layer is initialized with the output_layer_size x previous_layer_size.
 * The hidden layers are initialized with the next_layer_size x previous_layer_size.
 * 
 * @param nn The neural network.
 */
void nn_compile_layers(neural_network *nn){
    // iterate over the layers and initialize the weights matrices
    // begin with the input layer
    nn->layers[0]->weights = matrix_create_random(nn->layers[0]->nb_neurons, nn->layers[0]->input_size, -1, 1);

    // iterate over the hidden layers
    for(size_t i = 1; i < nn->nb_layers; i++)
        nn->layers[i]->weights = matrix_create_random(nn->layers[i]->nb_neurons, nn->layers[i]->input_size, -1, 1);
}   

/**
 * @brief Trains the neural network using the specified input and output matrices for the specified number of epochs.
 * 
 * @param nn The neural network.
 * @param X The input matrix.
 * @param T The target matrix.
 * @param epochs The number of epochs to train the network.
 */
void nn_train(neural_network *nn, matrix *X_data, matrix *T_data, size_t epochs){
    if(nn->nb_layers == 0){
            fprintf(stderr, "nn_train: Set the input layer first\n");
            exit(1);
    }
    if(nn->layers[nn->nb_layers - 1]->type != OUTPUT){
            fprintf(stderr, "nn_train: Set the output layer first\n");
            exit(1);
    }
    if(X_data->col != T_data->col){
            fprintf(stderr, "nn_train: Input and output matrices must have the same number of columns\n");
            exit(1);
    }

    // If code not compiled ie the weights of input layer are not setted
    if(((layer*)nn->layers[0])->input_size == 0){
    	// The only missing is the input size of the input layer (which is the number of raw of the input matrix)
    	// We therefore get the number of raw of the input matrix and set it as the input size of the input layer
    	// compile the layers by setting the weights matrices
        ((layer *) nn->layers[0])->input_size = X_data->row;
        nn_compile_layers(nn);
    }


    // Lists to store all results
    matrix** v_arr = malloc(nn->nb_layers * sizeof(matrix*));
    matrix** y_arr = malloc(nn->nb_layers * sizeof(matrix*));
    matrix** deltas_arr = malloc(nn->nb_layers * sizeof(matrix*));

    for(size_t e = 0; e < epochs; e++){
/*         // Normalize all weights by dividing by the absolute maximum value
        for(size_t i = 0; i < nn->nb_layers; i++){
            matrix *w = nn->layers[i]->weights;
            float max = -1;
            for(size_t j = 0; j < w->row * w->col; j++){
                if(fabs(w->data[j]) > max)
                    max = fabs(w->data[j]);
            }
            matrix_scalar_div_inplace(w, max);
        }
 */

        // select one X
        size_t index = e % X_data->col;
        matrix *X = matrix_get_col(X_data, index);
        matrix *T = matrix_get_col(T_data, index);

        // Forward propagation
        for(size_t i = 0; i < nn->nb_layers; i++){
            matrix *v;
            // Compute W*X
            if(i == 0)
                v = matrix_mul(nn->layers[i]->weights, X);
            else
                v = matrix_mul(nn->layers[i]->weights, y_arr[i - 1]);
            v_arr[i] = v;

            // Compute Y = f(W*X)
            y_arr[i] = matrix_get_copy(v);
            matrix_apply(y_arr[i], nn->layers[i]->activation);

            if(i == nn->nb_layers - 1)
                matrix_print(y_arr[i]);
        }

        // Backward propagation
        for(int i = nn->nb_layers - 1; i >= 0; i--){
            matrix *error;
            if((size_t) i == nn->nb_layers - 1){
                // Compute the output layer delta
                error = matrix_sub(T, y_arr[i]);

                if(nn->loss_function == CROSS_ENTROPY)
                    // delta = error
                    deltas_arr[i] = error;
                else{
                    // delta = error * f'(v)
                    deltas_arr[i] = matrix_get_copy(y_arr[i]);
                    matrix_apply(deltas_arr[i], nn->layers[i]->activation_prime);
                    matrix_dot_inplace(deltas_arr[i], error);
                }
            }else{
                // Compute the hidden layer delta
                matrix *w_t = matrix_transpose(nn->layers[i + 1]->weights);

                // delta = W_t * delta_next * f'(v)
                error = matrix_mul(w_t, deltas_arr[i + 1]);

                // sigmoid prime
                deltas_arr[i] = matrix_get_copy(y_arr[i]);
                matrix_apply(deltas_arr[i], nn->layers[i]->activation_prime);
                matrix_dot_inplace(error, y_arr[i]);

                // Free w_t and error
                matrix_destroy(w_t);
            }
        }

        // Update the weights
        for(size_t i = 0; i < nn->nb_layers; i++){
            matrix *y_t = matrix_transpose(i == 0 ? X : y_arr[i - 1]);
            matrix *dw = matrix_mul(deltas_arr[i], y_t);

            // dw_alpha = dw * alpha
            matrix_scalar_mul_inplace(dw, nn->learning_rate);
            // W = W + dw_alpha
            matrix_add_inplace(nn->layers[i]->weights, dw);

            // Free dw
            matrix_destroy(y_t);
            matrix_destroy(dw);
        }
    }

    // Inference
    matrix* output = nn_predict(nn, X_data);
    matrix_print(output);
    matrix_destroy(output);

    // Free all results
    for(size_t i = 0; i < nn->nb_layers; i++){
        matrix_destroy(v_arr[i]);
        matrix_destroy(y_arr[i]);
        matrix_destroy(deltas_arr[i]);
    }
    free(v_arr);
    free(y_arr);
    free(deltas_arr);
}

/**
 * @brief Predicts the output for the given input matrix using the neural network.
 * 
 * @param nn The neural network.
 * @param X The input matrix.
 * @return The predicted output matrix.
 */
matrix* nn_predict(neural_network *nn, matrix *X){
    if(nn->nb_layers == 0){
            fprintf(stderr, "nn_train: Set the input layer first\n");
            exit(1);
    }
    if(nn->layers[nn->nb_layers - 1]->type != OUTPUT){
            fprintf(stderr, "nn_train: Set the output layer first\n");
            exit(1);
    }

    // If code not compiled ie the weights of input layer are not setted
    if(((layer*)nn->layers[0])->input_size == 0){
    	// The only missing is the input size of the input layer (which is the number of raw of the input matrix)
    	// We therefore get the number of raw of the input matrix and set it as the input size of the input layer
    	// compile the layers by setting the weights matrices
        ((layer *) nn->layers[0])->input_size = X->row;
        nn_compile_layers(nn);
    }

    matrix** y_arr = malloc(nn->nb_layers * sizeof(matrix*));
    // Forward propagation
    for(size_t i = 0; i < nn->nb_layers; i++){
        matrix *v;
        // Compute W*X
        if(i == 0)
            v = matrix_mul(nn->layers[i]->weights, X);
        else
            v = matrix_mul(nn->layers[i]->weights, y_arr[i - 1]);

        // Compute Y = f(W*X)
        y_arr[i] = matrix_get_copy(v);
        matrix_apply(y_arr[i], nn->layers[i]->activation);

        if(i == nn->nb_layers - 1)
            matrix_print(y_arr[i]);
    }

    matrix* output = matrix_get_copy(y_arr[nn->nb_layers - 1]);

    // Free all results
    for(size_t i = 0; i < nn->nb_layers; i++)
        matrix_destroy(y_arr[i]);
    free(y_arr);

    return output;
}

void nn_display_layers(neural_network *nn){
    printf("Neural Network : -> ");

    for(size_t i = 0; i < nn->nb_layers; i++){
        printf("Layer %zu :\n", i);
        matrix_print(nn->layers[i]->weights);
    }

    for(size_t i = 0; i < nn->nb_layers; i++)
        printf("(%zu, %zu) ->", nn->layers[i]->nb_neurons, nn->layers[i]->input_size);
    
    puts("");
}

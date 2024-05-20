CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
TARGET = main

all: $(TARGET)

$(TARGET): main.c src/Matrix/matrix.c src/NeuralNetwork/neuralNetwork.c src/list/list.c src/list/list_iterator.c src/list/list_node.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

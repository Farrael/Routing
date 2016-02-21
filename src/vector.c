#include <stdio.h>
#include <stdlib.h>

/* Internal dependencies */
#include "vector.h"

/**
* Create a new vector
*/
Vector* vector_create() {
	Vector* vector = malloc(sizeof(Vector));
	vector->data   = malloc(sizeof(int) * VECTOR_INITIAL_CAPACITY);

	vector->size     = 0;
	vector->capacity = VECTOR_INITIAL_CAPACITY;

	return vector;
}

/**
* Add an element to the vector
*/
void vector_append(Vector *vector, int value) {
	vector_check_size(vector);

	vector->data[vector->size++] = value;
}

/**
* Get an element by his id
*/
int vector_get(Vector *vector, int index) {
	if (index >= vector->size || index < 0) {
		printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
		exit(1);
	}

	return vector->data[index];
}

/**
* Add an element to an id
*/
void vector_set(Vector *vector, int index, int value) {
	while (index >= vector->size)
		vector_append(vector, 0);

	vector->data[index] = value;
}

/**
* Update the vector size if needed
*/
void vector_check_size(Vector *vector) {
	if (vector->size >= vector->capacity) {
		vector->capacity *= 2;
		vector->data      = realloc(vector->data, sizeof(int) * vector->capacity);
	}
}

/**
* Free the memory used by the vector
*/
void vector_free(Vector *vector) {
	free(vector->data);
}
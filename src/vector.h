/*
vector.h
-----

Role : Vector class (Dynamic Array)
*/

#ifndef H_VECTOR
#define H_VECTOR
#define VECTOR_INITIAL_CAPACITY 50

	/**
	 * Vector structure
	 */
	typedef struct {
		int size;
		int capacity;
		int *data;
	} Vector;

	/**
	 * Functions
	 */
	Vector* vector_create();
	void    vector_free(Vector *vector);

	void vector_append(Vector *vector, int value);
	int  vector_get(Vector *vector, int index);
	void vector_set(Vector *vector, int index, int value);

	void vector_check_size(Vector *vector);

#endif
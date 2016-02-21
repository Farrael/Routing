/*
structure.h
-----

Role : Global structures used to solve the route
*/

#ifndef H_STRUCTURE
#define H_STRUCTURE

#include "vector.h"

	/**
	 * Node structure
	 *   type : 0 -> Empty
	 *          1 -> Wall
	 *          2 -> Link
	 */
	typedef struct Node {
		int type;
		int value;
	} Node;

	/**
	 * Grid structure
	 */
	typedef struct Grid {
		// Size
		int width;
		int height;

		// Layer
		Node** cell;

		// Nodes positions
		Vector* nodes;
	} Grid;

	/**
	 * Functions
	 */
	Grid* create_grid(const int, const int);

	void display_grid(Grid*);

	void free_grid(Grid*);

#endif

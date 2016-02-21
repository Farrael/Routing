#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Internal dependencies */
#include "structure.h"
#include "vector.h"

/**
 * Create a new grid from dimension
 * 
 * @param width  : Width of the table
 * @param height : Height of the table
 *
 * @return Grid created
 */
Grid* create_grid(int width, int height) {
	Grid* grid  = (Grid*) malloc(sizeof(Grid));
	grid->nodes = vector_create();

	// Register table size
	grid->width  = width;
	grid->height = height;

	Node* temp;

	// Create the cells
	grid->cell = (Node**) malloc(height * sizeof(Node*));
	temp       = (Node*)  malloc(height * width * sizeof(Node));

	int i;
	for(i = 0; i < height; i++)
		grid->cell[i] = temp + (i * width);

	return grid;
}

/**
 * Display the grid on the screen
 * 
 * @param grid : Grid to display
 */
void display_grid(Grid* grid) {
	int i, j, type;

	for(i = 0; i < grid->height; i++) {
		for(j = 0; j < grid->width; j++) {
			type = grid->cell[i][j].type;

			if(type == 0)
				printf("0 ");
			else if(type == 1)
				printf("X ");
			else
				printf("%d ", grid->cell[i][j].value);
		}
		printf("\n");
	}
}

/**
 * Free memory allocated for a grid
 *
 * @param grid : Grid to free
 */
void free_grid(Grid* grid) {
	// Free cells
	free(grid->cell[0]);
	free(grid->cell);

	// Free the vector
	vector_free(grid->nodes);

	// Free structure
	free(grid);
}
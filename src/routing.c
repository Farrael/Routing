#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Internal dependencies */
#include "routing.h"
#include "structure.h"
#include "utils.h"
#include "vector.h"

int main(int argc, char *argv[]) {
	Grid* grid;

	// Display logo
	logo();

    // Detect arguments
	if(argc >= 2)
    	grid = setup_from_file(argv[1]);
	else
		grid = setup_from_user();

	if(grid != NULL) {
		// Display grid information
		printf("\n[Table Informations]\n");
		printf("   - Width    : %d\n   - Height   : %d\n", grid->width, grid->height);
		printf("   - Elements : %d\n   - Nodes    : %d\n", grid->width * grid->height, grid->nodes->size);

		printf("\n[Table Content]\n");
		display_grid(grid);

		// Find path between nodes
		printf("\n[Path Finder]\n");
		find_path(grid);

		// Free the grid
		free_grid(grid);
	}

    return 0;
}

/*************************************************/
/*                   GRID-SETUP                  */
/*************************************************/

/**
 * Setup grid from user input
 */
Grid* setup_from_user() {
	int width, height;

	// Ask for grid size
	printf("\n[Grid parameters]\n");
	printf("Width  : ");
    scanf("%d", &width);
	printf("Height : ");
    scanf("%d", &height);

	// Create grid
    Grid* grid = create_grid(width, height);

    // @Todo
    // Create grid here

    return grid;
}

/**
 * Setup grid from file
 * 
 * @param name : Name of the file
 * 
 * @return Grid created
 */
Grid* setup_from_file(char *name) {
	int width, height, i;

	// Retrieve file info
	if(read_file(name, &width, &height) == 0) {
		if(width == -1)
			printf("Can't open the given file : %s\n", name);
		else
			printf("The file dimension is wrong at line %d, excepted : %d", height, width);

		return NULL;
	}

	// Open the file
	FILE* file = fopen(name, "r");

	// Can't open the file
	if(file == NULL)
		return NULL;

	Grid* grid = create_grid(width, height);
	i = 0;

	char read[3];
	int value;
	while (fscanf(file, "%s", read) != EOF) {
		if(read[0] == '0')
			grid->cell[0][i].type = 0;
		else if(read[0] == 'X')
			grid->cell[0][i].type = 1;
		else {
			toInteger(read, &value);

			if(value > 0) {
				grid->cell[0][i].type  = 2;
				grid->cell[0][i].value = value;

				vector_append(grid->nodes, i);
			} else {
				printf("Error on file... can't parse cell[%d][%d] !\n", i / width, i % width);
				return NULL;
			}
		}

		i++;
	}

	fclose(file);

	return grid;
}

/*************************************************/
/*                  PATH-FINDER                  */
/*************************************************/
 
/**
 * Fin path between grid nodes
 * 
 * @param  grid : Grid to work on
 */
void find_path(Grid* grid) {
	int i, j, x, y, value;

	int  size   = grid->width * grid -> height;
	int* result = (int*) malloc(size * sizeof(int));
	for(i = 0; i < size; i++)
		result[i] = 0;

	// Retrieve path between nodes
	for(i = 0; i < grid->nodes->size; i++) {
		x = grid->nodes->data[i];

		if(grid->cell[0][x].type == 3)
			continue;

		value = grid->cell[0][x].value;
		for(j = 0; j < grid->nodes->size; j++) {
			y = grid->nodes->data[j];

			if(grid->cell[0][y].value == value && i != j)
				_pre_wave(x, y, grid, result, value);
		}
	}

	// Display the final grid
	printf("\nResult : \n");
	int t;
	for(t = 0; t < size; t++) {
		if(t != 0 && t % grid->width == 0)
			printf("\n");

		int type = grid->cell[0][t].type;
		if(type == 0) {
			int top, bottom;
			bit_decode(result[t], &top, &bottom);
			printf("[%02d|%02d] ", top, bottom);
		} else if(type == 1) {
			printf(" XXXXX  ");
		} else {
			printf("  [%d]   ", grid->cell[0][t].value);
		}
	}
	printf("\n\n");

	free(result);
}

/**
 * Find path between the start node to the end node
 * 
 * @param start  : Index of the start node
 * @param end    : Index of the end node
 * @param grid   : Grid to work on
 * @param result : Array that contain the result
 * @param value  : Index of the current path
 */
void _pre_wave(int start, int end, Grid* grid, int result[], int value) {
	int*  flag;
	int   i, size, found, top, bottom;

	size = grid->width * grid -> height;

	// Allocate memory for handle ID of nodes
	flag = (int*) malloc(size * sizeof(int));
	for(i = 0; i < size; i++)
		flag[i] = -1;

	int print_size = round(1 + log(i) / log(10));

	// Create vector
	Vector* vector = vector_create();
	vector_append(vector, start);

	// Wave with pile
	i = 0;
	do {
		found = _wave(grid, vector_get(vector, i), end, flag, result, vector);
	} while(found == 0 && ++i < vector->size);

	// Retrieve path
	if(found) {
		i = vector_get(vector, i);

		// Set nodes to already linked
		grid->cell[0][start].type = 3;
		grid->cell[0][end].type   = 3;

		while(i != start) {
			// Encode value on the right side
			bit_decode(result[i], &top, &bottom);
			if(top == 0) 
				top = value;
			else
				bottom = value;
			bit_encode(&result[i], top, bottom);
			i = flag[i];
		}

		printf("   - [%0*d] -> [%0*d] : Valide\n", print_size, start, print_size, end);
	} else {
		printf("   - [%0*d] -> [%0*d] : Invalide\n", print_size, start, print_size, end);
	}

	// Display grid on error
	int t;
	for(t = 0; t < size; t++) {
		if(t != 0 && t % grid->width == 0)
			printf("\n");

		if(t == start) {
			printf("%c[1;32m START  %c[0m", 27, 27);
		} else if(t == end) {
			printf("%c[1;32m  END   %c[0m", 27, 27);
		} else {
			bit_decode(result[t], &top, &bottom);
			if(top == value ||bottom == value)
				printf("%c[1;31m[%02d|%02d]%c[0m ", 27, top, bottom, 27);
			else
				printf("[%02d|%02d] ", top, bottom);
		}
	}
	printf("\n\n");

	// Free allocated memory
	vector_free(vector);
	free(flag);
}

/**
 * Recursive algorithme that check neighbors validity
 * 
 * @param  current : Index of the current node
 * @param  end     : Index of the end node
 * @param  flag    : Array of node already checked
 * @param  result  : Array that contain the result
 * @param  vector  : Vector that contain the list of node to check
 * 
 * @return 1 if the wave reach the end
 * 
 */
int _wave(Grid* grid, int current, int end, int flag[], int result[], Vector* vector) {
	int found, x, y, i, j, id, type;

	// Condition d'arrêt
	if(current == end)
		return 1;

	// Position on the grid
	x = current % grid->width;
	y = current / grid->width;

	// Check neighbours
	found = 0;
	i     = -1;
	while(i <= 1 && found == 0) {
		if(y + i >= 0 && y + i < grid->height) {

			j = -1;
			while(j <= 1 && found == 0) {
				id = (y + i) * grid->width + (x + j);

				if(x + j >= 0 && x + j < grid->width && flag[id] == -1 && !(i == 0 && j == 0)) {
					flag[id] = current;
					type     = grid->cell[0][id].type;

					// Wall or Wrong pin
					if(type == 1 || (type == 2 && id != end))
						continue;

					int top, bottom;
					bit_decode(result[id], &top, &bottom);
					if(top != 0 && bottom != 0)
						continue;

					vector_append(vector, id);
				}

				j++;
			}
		}

		i++;
	}

	return 0;
}

/**
 * Display main logo
 */
void logo(){
	printf("  ____             _   _                    \n");
	printf(" |  _ \\ ___  _   _| |_(_)_ __   __ _       \n");
	printf(" | |_) / _ \\| | | | __| | '_ \\ / _` |     \n");
	printf(" |  _ < (_) | |_| | |_| | | | | (_| |       \n");
	printf(" |_| \\_\\___/ \\__,_|\\__|_|_| |_|\\__, |  \n");
	printf("                          v0.1 |___/        \n\n");
}
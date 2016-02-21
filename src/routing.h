/*
routing.h
-----

Role : Main application file
*/

#ifndef H_ROUTING
#define H_ROUTING

#include "structure.h"
#include "vector.h"

	// Setup grid
	Grid* setup_from_user();
	Grid* setup_from_file(char*);

    // Path finder
    void find_path(Grid*);
    void _pre_wave(int, int, Grid*, int[], int);
    int  _wave(Grid*, int, int, int[], int[], Vector*);
    
    // Print logo
    void logo();

#endif
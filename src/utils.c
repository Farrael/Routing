#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/* Internal dependencies */
#include "utils.h"

/* Constant */
#define BIT_SIZE 15
#define BIT_MASK 0x7FFF


/**
 * Returne informations from a file
 */
int read_file(char* name, int* width, int* height) {
	int c, space, overflow, statu;
	FILE* file = fopen(name, "r");

	// Set statu to succes
	statu = 1;

	if(file) {
		// Default value
		*width = *height = overflow = 0;
		space  = 1;

		while (1) {
			c = getc(file);

			// End of file
			if(c == EOF) {
				(*height)++;
				break;
			}

			// End of line
			else if(c == '\n') {
				(*height)++;
				overflow = 0;
			}

			// Detecte space
			else if(c == ' ') {
				space = 1;
			}

			// Calculate width
			else {
				if(*height == 0 && space == 1) {
					(*width)++;
					space = 0;
				} else if(*height > 0) {
					if(++overflow > *width) {
						*width = overflow;
						statu = 0;
						break;
					}
				}
			}
		}

		// Close the file
		fclose(file);
	} else {
		*width = *height = -1;
		statu = 0;
	}

	return statu;
}

/**
 * Converts array of char into integer.
 */
void toInteger(char* str, int* n){
	unsigned int size = strlen(str);
	(*n) = 0;
	int first = 1;

	unsigned int i;
	for(i = 0; i < size; i++){
		if(!isdigit(str[i]))
			first = 0;

		if(first == 1 && str[i] >= '0' && (char)str[i] <= '9'){
			(*n) *= 10;
			(*n) += (str[i] - '0');
		}
	}
}

/**
 * Encode data on Integer
 * 
 * @param output : Encoded number
 * @param right  : First data to encode
 * @param left   : Seconde data to encode
 */
void bit_encode(int* output, const int right, const int left) {
	(*output) = (left << BIT_SIZE) | right;
}

/**
 * Decode data from Integer
 * 
 * @param intput : Encoded Number
 * @param right  : First data decoded
 * @param left   : Seconde data decoded
 */
void bit_decode(int input, int* right, int* left) {
	(*right) = input & BIT_MASK;
	(*left)  = (input >> BIT_SIZE) & BIT_MASK;
}
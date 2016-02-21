/*
utils.h
-----

Role : Utility function
*/

#ifndef H_UTILS
#define H_UTILS

	int  read_file(char*, int*, int*);
	void toInteger(char*, int*);

	/* Encode Integer */
	void bit_encode(int* , const int, const int );
	void bit_decode(int  , int*     , int*);

#endif
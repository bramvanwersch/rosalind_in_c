#include <stdio.h>

#include "general_functions.h"


FILE *file_pointer;

int main_complementing_a_strand_of_DNA(char *argv[]) {
	// read the input file into memory
	file_pointer = open_file(argv[2], "r");
	fclose(file_pointer);
	return 0;
}
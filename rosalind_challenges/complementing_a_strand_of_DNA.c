#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general_functions.h"

void complement_DNA();

FILE *input_file_pointer;
FILE *output_file_pointer;

int main_complementing_a_strand_of_DNA(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	complement_DNA();
	fclose(input_file_pointer);
	fclose(output_file_pointer);
	return 0;
}

void complement_DNA() {
	char *file_string;
	char ch;
	int string_len;

	file_string = read_file(input_file_pointer);
	string_len = strlen(file_string);

	// move over the string in reverse and put the complement characters in the output_file
	while(--string_len >= 0) {
		ch = file_string[string_len];
		switch (ch) {
		case 'A':
			fputc('T', output_file_pointer);
			break;
		case 'C':
			fputc('G', output_file_pointer);
			break;
		case 'G':
			fputc('C', output_file_pointer);
			break;
		case 'T':
			fputc('A', output_file_pointer);
			break;
		default:
			fprintf(stderr, "Invalid character %d\n", ch);
			exit(1);
		}
	}
}
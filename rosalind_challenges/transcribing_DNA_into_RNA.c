#include <stdio.h>

#include "general_functions.h"

FILE *input_file_pointer;
FILE *output_file_pointer;

void make_RNA();

int main_transcribing_DNA_into_RNA(char *argv[]) {

	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	make_RNA();

	return 0;
}

void make_RNA() {
	// simply replace all occurances of T with U
	char ch;

	while ((ch = fgetc(input_file_pointer)) != EOF) {
		if (ch == 'T') {
			ch = 'U';
		}
		fputc(ch, output_file_pointer);
	}
}
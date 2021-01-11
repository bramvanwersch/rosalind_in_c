#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "general_functions.h"

void count_nucleotides();

FILE *input_file_pointer;
int dna_counts[] = { 0, 0, 0, 0 };


int main_counting_DNA_nucleotides(char *argv[]){
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	count_nucleotides();
	fclose(input_file_pointer);

	input_file_pointer = open_file("output.txt", "w");
	// print result into file
	for (int i = 0; i < 4; i++) {
		printf("%d%s", dna_counts[i], (i < 3) ? " " : "\n");
		fprintf(input_file_pointer, "%d%s", dna_counts[i], (i < 3) ? " " : "");
	}
	return 0;
}

void count_nucleotides() {
	char ch;

	while ((ch = fgetc(input_file_pointer)) != EOF) {
		switch (ch) {
		case 'A':
			dna_counts[0]++;
			break;
		case 'C':
			dna_counts[1]++;
			break;
		case 'G':
			dna_counts[2]++;
			break;
		case 'T':
			dna_counts[3]++;
			break;
		default:
			fprintf(stderr, "Invalid character %d\n", ch);
			exit(1);
		}
	}
}
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "general_functions.h"

int main(int argc, char *argv[])
{
	char ch;
	FILE *input_file_pointer;
	int dna_counts[] = { 0, 0, 0, 0 };

	if (argc < 2) {
		printf("Expected at least 2 files got %d", argc);
		exit(1);
	}
	input_file_pointer = open_file(argv[1], "r");
	while ((ch = fgetc(input_file_pointer)) != EOF)
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
	fclose(input_file_pointer);

	input_file_pointer = open_file("output.txt", "w");
	// print result into file
	for (int i = 0; i < 4; i++) {
		printf("%d%s", dna_counts[i], (i < 3) ? " " : "\n");
		fprintf(input_file_pointer, "%d%s", dna_counts[i], (i < 3) ? " " : "");
	}
	return 0;
}


#include <stdio.h>
#include <string.h>
#include "string_algorithms.h"


int main(int argc, char *argv[]) {
	// expected argv argument argv[1] = name of challenge, argv[2:] = other arguments
	int result;

	// make sure that the input file is provided
	if (argc < 3) {
		printf("Expected at least 3 argv arguments got %d\n", argc);
		return 1;
	}

	// ideal case this would be some sort of hashtable lookup
	if (strcmp(argv[1], "counting_DNA_nucleotides") == 0) {
		result = counting_DNA_nucleotides(argv);
	}
	else if (strcmp(argv[1], "transcribing_DNA_into_RNA") == 0) {
		result = transcribing_DNA_into_RNA(argv);
	}
	else if (strcmp(argv[1], "complementing_a_strand_of_DNA") == 0) {
		result = complementing_a_strand_of_DNA(argv);
	}
	else if (strcmp(argv[1], "computing_GC_content") == 0) {
		result = computing_GC_content(argv);
	}
	else {
		printf("Unexpected solution name %s\n", argv[1]);
		return 2;
	}
	printf("Done running the challenge '%s' with exit code %d.\n", argv[1], result);
	return 0;
}

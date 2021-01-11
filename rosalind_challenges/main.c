#include <stdio.h>
#include <string.h>

int main_counting_DNA_nucleotides(char *argv[]);
int main_transcribing_DNA_into_RNA(char *argv[]);

int main(int argc, char *argv[]) {
	// expected argv argument argv[1] = name of challenge, argv[2:] = other arguments

	// make sure that the input file is provided
	if (argc < 3) {
		printf("Expected at least 3 argv arguments got %d\n", argc);
		return 1;
	}

	// ideal case this would be some sort of hashtable lookup
	if (strcmp(argv[1], "counting_DNA_nucleotides") == 0) {
		int result = main_counting_DNA_nucleotides(argv);
	}
	else if (strcmp(argv[1], "transcribing_DNA_into_RNA") == 0) {
		int result = main_transcribing_DNA_into_RNA(argv);
	}
	else {
		printf("Unexpected solution name %s\n", argv[1]);
		return 2;
	}
	printf("Done running the challenge '%s'.\n", argv[1]);
	return 0;
}

#include <stdio.h>
#include <string.h>
#include "string_algorithms.h"
#include "allignment_algorithms.h"
#include "combinatorics_algorithms.h"


int test(char *argv[]) {
	test_hash_table();
	test_linked_list();
	test_set();
	return 0;
}

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
	else if (strcmp(argv[1], "counting_point_mutations") == 0) {
		result = counting_point_mutations(argv);
	}
	else if (strcmp(argv[1], "translating_RNA_into_protein") == 0) {
		result = translating_RNA_into_protein(argv);
	}
	else if (strcmp(argv[1], "inferring_mRNA_from_protein") == 0) {
		result = inferring_mRNA_from_protein(argv);
	}
	else if (strcmp(argv[1], "finding_a_motif_in_DNA") == 0) {
		result = finding_a_motif_in_DNA(argv);
	}
	else if (strcmp(argv[1], "RNA_splicing") == 0) {
		result = RNA_splicing(argv);
	}
	else if (strcmp(argv[1], "finding_a_spliced_motif") == 0) {
		result = finding_a_spliced_motif(argv);
	}
	else if (strcmp(argv[1], "transitions_and_transversions") == 0) {
		result = transitions_and_transversions(argv);
	}
	else if (strcmp(argv[1], "locating_restriction_sites") == 0) {
		result = locating_restriction_sites(argv);
	}
	else if (strcmp(argv[1], "open_reading_frames") == 0) {
		result = open_reading_frames(argv);
	}
	else if (strcmp(argv[1], "test") == 0) {
		result = test(argv);
	}
	else {
		printf("Unexpected solution name %s\n", argv[1]);
		return 2;
	}
	printf("Done running the challenge '%s' with exit code %d.\n", argv[1], result);
	return 0;
}

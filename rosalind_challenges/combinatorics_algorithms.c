#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general_functions.h"
#include "string_algorithms.h"

FILE *input_file_pointer;
FILE *output_file_pointer;

HashTable *get_aa_codon_numbers();
long long mRNA_possibilities(char *protein);

HashTable *get_aa_codon_numbers() {
	HashTable *aa_codon_numbers = new_hash_table(81);
	aa_codon_numbers->add(aa_codon_numbers, "A", "4");
	aa_codon_numbers->add(aa_codon_numbers, "R", "6");
	aa_codon_numbers->add(aa_codon_numbers, "N", "2");
	aa_codon_numbers->add(aa_codon_numbers, "D", "2");
	aa_codon_numbers->add(aa_codon_numbers, "C", "2");
	aa_codon_numbers->add(aa_codon_numbers, "Q", "2");
	aa_codon_numbers->add(aa_codon_numbers, "E", "2");
	aa_codon_numbers->add(aa_codon_numbers, "G", "4");
	aa_codon_numbers->add(aa_codon_numbers, "H", "2");
	aa_codon_numbers->add(aa_codon_numbers, "M", "1");
	aa_codon_numbers->add(aa_codon_numbers, "I", "3");
	aa_codon_numbers->add(aa_codon_numbers, "L", "6");
	aa_codon_numbers->add(aa_codon_numbers, "K", "2");
	aa_codon_numbers->add(aa_codon_numbers, "F", "2");
	aa_codon_numbers->add(aa_codon_numbers, "P", "4");
	aa_codon_numbers->add(aa_codon_numbers, "S", "6");
	aa_codon_numbers->add(aa_codon_numbers, "T", "4");
	aa_codon_numbers->add(aa_codon_numbers, "W", "1");
	aa_codon_numbers->add(aa_codon_numbers, "Y", "2");
	aa_codon_numbers->add(aa_codon_numbers, "V", "4");
	// stop codon
	aa_codon_numbers->add(aa_codon_numbers, "X", "3");
	return aa_codon_numbers;
}

int inferring_mRNA_from_protein(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	char *protein = read_file(input_file_pointer);
	long long result = mRNA_possibilities(protein);
	printf("Modulo 1.000.000 possibilities: %lld\n", result);
	fprintf(output_file_pointer, "%lld", result);
	fclose(output_file_pointer);
	fclose(input_file_pointer);
	return 0;
}

long long mRNA_possibilities(char *protein) {
	int total = 1;
	long long modulo_total = 1;
	char ch;
	char aa[2];
	HashTable *codon_numbers = get_aa_codon_numbers();
	while (ch = *protein++) {
		aa[0] = ch;
		aa[1] = '\0';
		int codon_number = atoi(codon_numbers->get(codon_numbers, aa));
		total *= codon_number;
		if (total > 1000000) {
			modulo_total *= (total % 1000000);
			if (modulo_total > 1000000) {
				modulo_total = modulo_total % 1000000;
			}
			total = 1;
		}
	}
	modulo_total *= total % 1000000;
	modulo_total *= 3;
	return modulo_total = modulo_total % 1000000;
}
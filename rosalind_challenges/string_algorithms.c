#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general_functions.h"
#include "string_algorithms.h"

void complement_DNA();

FILE *input_file_pointer;
FILE *output_file_pointer;
int dna_counts[] = { 0, 0, 0, 0 };


int counting_DNA_nucleotides(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	count_nucleotides();
	fclose(input_file_pointer);

	output_file_pointer = open_file("output.txt", "w");
	// print result into file
	for (int i = 0; i < 4; i++) {
		printf("%d%s", dna_counts[i], (i < 3) ? " " : "\n");
		fprintf(output_file_pointer, "%d%s", dna_counts[i], (i < 3) ? " " : "");
	}
	fclose(output_file_pointer);
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

int transcribing_DNA_into_RNA(char *argv[]) {

	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	make_RNA();
	fclose(input_file_pointer);
	fclose(output_file_pointer);

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

int complementing_a_strand_of_DNA(char *argv[]) {
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
	while (--string_len >= 0) {
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

int computing_GC_content(char *argv[]) {
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	char name[100]; //sufficiently large buffer for any possible name
	int counts[2] = {0, 0}; //gc count, total count
	float best_gc = 0;
	char best_name[100];
	char *best_name_pointer = best_name;
	LinkedList *lines = read_lines(input_file_pointer);

	while (lines->next != NULL) {
		if (lines->value[0] == '>') {
			float gc_content = (counts[0] / (float)counts[1]);
			if (gc_content > best_gc) {
				best_gc = gc_content;
				strcpy_s(best_name, 100, name);
			}
			strcpy_s(name, 100, lines->value);
			counts[0] = 0;
			counts[1] = 0;
		}
		else {
			count_GC_content(counts, lines->value);
		}
		lines = lines->next;
	}

	float gc_content = (counts[0] / (float)counts[1]);
	if (gc_content > best_gc) {
		best_gc = gc_content;
		strcpy_s(best_name, 100, name);
	}

	best_name_pointer++;
	printf("%s\n%f\n", best_name_pointer, best_gc * 100);
	fprintf(output_file_pointer, "%s\n%f", best_name_pointer, best_gc * 100);

	fclose(input_file_pointer);
	fclose(output_file_pointer);
	return 0;
}

void count_GC_content(int *counts, char *line) {
	char ch;
	while (ch = *line++) {
		switch (ch){
		case 'C':
			counts[0]++;
			counts[1]++;
			break;
		case 'G':
			counts[0]++;
			counts[1]++;
			break;
		case 'A':
			counts[1]++;
			break;
		case 'T':
			counts[1]++;
			break;
		default:
			fprintf(stderr, "Invalid character %d\n", ch);
			exit(1);
		}
	}
}

HashTable *create_aa_table() {
	// setup the amino acid table
	HashTable *amino_acid_table = new_hash_table(81); // save amount of space, and because of the limited alphabet 81 works out better
	amino_acid_table->add(amino_acid_table, "GCU", "A");
	amino_acid_table->add(amino_acid_table, "GCC", "A");
	amino_acid_table->add(amino_acid_table, "GCA", "A");
	amino_acid_table->add(amino_acid_table, "GCG", "A");

	amino_acid_table->add(amino_acid_table, "CGU", "R");
	amino_acid_table->add(amino_acid_table, "CGC", "R");
	amino_acid_table->add(amino_acid_table, "CGA", "R");
	amino_acid_table->add(amino_acid_table, "CGG", "R");
	amino_acid_table->add(amino_acid_table, "AGA", "R");
	amino_acid_table->add(amino_acid_table, "AGG", "R");

	amino_acid_table->add(amino_acid_table, "AAU", "N");
	amino_acid_table->add(amino_acid_table, "AAC", "N");

	amino_acid_table->add(amino_acid_table, "GAU", "D");
	amino_acid_table->add(amino_acid_table, "GAC", "D");

	amino_acid_table->add(amino_acid_table, "UGU", "C");
	amino_acid_table->add(amino_acid_table, "UGC", "C");

	amino_acid_table->add(amino_acid_table, "CAA", "Q");
	amino_acid_table->add(amino_acid_table, "CAG", "Q");

	amino_acid_table->add(amino_acid_table, "GAA", "E");
	amino_acid_table->add(amino_acid_table, "GAG", "E");

	amino_acid_table->add(amino_acid_table, "GGU", "G");
	amino_acid_table->add(amino_acid_table, "GGC", "G");
	amino_acid_table->add(amino_acid_table, "GGA", "G");
	amino_acid_table->add(amino_acid_table, "GGG", "G");

	amino_acid_table->add(amino_acid_table, "CAU", "H");
	amino_acid_table->add(amino_acid_table, "CAC", "H");

	amino_acid_table->add(amino_acid_table, "AUG", "M");

	amino_acid_table->add(amino_acid_table, "AUU", "I");
	amino_acid_table->add(amino_acid_table, "AUC", "I");
	amino_acid_table->add(amino_acid_table, "AUA", "I");

	amino_acid_table->add(amino_acid_table, "CUU", "L");
	amino_acid_table->add(amino_acid_table, "CUC", "L");
	amino_acid_table->add(amino_acid_table, "CUA", "L");
	amino_acid_table->add(amino_acid_table, "CUG", "L");
	amino_acid_table->add(amino_acid_table, "UUA", "L");
	amino_acid_table->add(amino_acid_table, "UUG", "L");

	amino_acid_table->add(amino_acid_table, "AAA", "K");
	amino_acid_table->add(amino_acid_table, "AAG", "K");

	amino_acid_table->add(amino_acid_table, "UUU", "F");
	amino_acid_table->add(amino_acid_table, "UUC", "F");

	amino_acid_table->add(amino_acid_table, "CCU", "P");
	amino_acid_table->add(amino_acid_table, "CCC", "P");
	amino_acid_table->add(amino_acid_table, "CCA", "P");
	amino_acid_table->add(amino_acid_table, "CCG", "P");

	amino_acid_table->add(amino_acid_table, "UCU", "S");
	amino_acid_table->add(amino_acid_table, "UCC", "S");
	amino_acid_table->add(amino_acid_table, "UCA", "S");
	amino_acid_table->add(amino_acid_table, "UCG", "S");
	amino_acid_table->add(amino_acid_table, "AGU", "S");
	amino_acid_table->add(amino_acid_table, "AGC", "S");

	amino_acid_table->add(amino_acid_table, "ACU", "T");
	amino_acid_table->add(amino_acid_table, "ACC", "T");
	amino_acid_table->add(amino_acid_table, "ACA", "T");
	amino_acid_table->add(amino_acid_table, "ACG", "T");

	amino_acid_table->add(amino_acid_table, "UGG", "W");

	amino_acid_table->add(amino_acid_table, "UAU", "Y");
	amino_acid_table->add(amino_acid_table, "UAC", "Y");

	amino_acid_table->add(amino_acid_table, "GUU", "V");
	amino_acid_table->add(amino_acid_table, "GUC", "V");
	amino_acid_table->add(amino_acid_table, "GUA", "V");
	amino_acid_table->add(amino_acid_table, "GUG", "V");

	// the 3 stop codons
	amino_acid_table->add(amino_acid_table, "UAA", "X");
	amino_acid_table->add(amino_acid_table, "UGA", "X");
	amino_acid_table->add(amino_acid_table, "UAG", "X");

	return amino_acid_table;
}

int translating_RNA_into_protein(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	char *rna_string = read_file(input_file_pointer);

	char *protein_strand = translate_RNA(rna_string);
	fprintf(output_file_pointer, "%s", protein_strand);

	fclose(input_file_pointer);
	fclose(output_file_pointer);
	return 0;
}

char *translate_RNA(char *rna_string) {
	HashTable *aa_table = create_aa_table();
	char ch;
	char codon[4];
	int total_aas = strlen(rna_string) / 3;
	char *protein_strand;

	protein_strand = (char *) malloc(sizeof(char) * (total_aas + 1)); // to include the '\0'

	int codon_index = 0;
	int protein_stand_index = 0;
	while (ch = *rna_string++) {
		codon[codon_index] = ch;
		codon_index++;
		if (codon_index == 3) {
			codon[codon_index] = '\0';
			char *value = aa_table->get(aa_table, codon);
			if (value[0] != 'X') {
				protein_strand[protein_stand_index] = value[0];
				protein_stand_index++;
				codon_index = 0;
			}
		}
	}
	protein_strand[protein_stand_index] = '\0';
	return protein_strand;
}
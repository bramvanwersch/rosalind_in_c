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
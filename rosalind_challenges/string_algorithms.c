#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "general_functions.h"
#include "string_algorithms.h"

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
	LinkedEntry *line = lines->root;

	while (line->next != NULL) {
		char *entry_value = (char *)line->value;
		if (entry_value[0] == '>') {
			float gc_content = (counts[0] / (float)counts[1]);
			if (gc_content > best_gc) {
				best_gc = gc_content;
				strcpy_s(best_name, 100, name);
			}
			strcpy_s(name, 100, entry_value);
			counts[0] = 0;
			counts[1] = 0;
		}
		else {
			count_GC_content(counts, entry_value);
		}
		line = line->next;
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
	HashTable *amino_acid_table = new_hash_table(81, 's'); // save amount of space, and because of the limited alphabet 81 works out better
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

	char *protein_strand = RNA_to_protein(rna_string);
	fprintf(output_file_pointer, "%s", protein_strand);

	fclose(input_file_pointer);
	fclose(output_file_pointer);
	return 0;
}

char *RNA_to_protein(char *rna_string) {
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

			}
			protein_stand_index++;
			codon_index = 0;
		}
	}
	protein_strand[protein_stand_index] = '\0';
	return protein_strand;
}

LinkedList *find_motif_start_locations(char *search_string, char *motif);

int finding_a_motif_in_DNA(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	LinkedList *input_lines = read_lines(input_file_pointer);
	LinkedEntry *line_entry = input_lines->root;

	char *search_string = (char *) line_entry->value;
	char *motif = (char *) line_entry->next->value;
	LinkedList *start_locations = find_motif_start_locations(search_string, motif);
	LinkedEntry *location = start_locations->root;
	while (location->value != NULL) {
		int number = *(int *)location->value;
		if (location->next->value == NULL) {
			fprintf(output_file_pointer, "%d", number);
		}
		else{
			fprintf(output_file_pointer, "%d ", number);
		}
		
		location = location->next;
	}

	fclose(output_file_pointer);
	fclose(input_file_pointer);
	return 0;
}

LinkedList *find_motif_start_locations(char *search_string, char *motif) {
	int search_len = strlen(search_string);
	int motif_len = strlen(motif);
	int match;

	LinkedList *start_locations = new_linked_list('i');

	for (int search_index = 0; search_index < search_len; search_index++) {
		match = True;
		for (int motif_index = 0; motif_index < motif_len; motif_index++) {
			if (search_string[search_index + motif_index] != motif[motif_index]) {
				match = False;
				break;
			}
		}
		if (match == True) {
			int *motif_location = (int *)malloc(sizeof(int));
			*motif_location =  search_index + 1;
			start_locations->add(start_locations, motif_location);
		}
	}
	return start_locations;
}

char *remove_introns(char *main_strand, char **intron_sequences, int no_seqs);
char **get_sorted_intron_sequences(LinkedList *all_sequences);
char *remove_introns_and_translate(LinkedList *input_lines);
HashTable *get_hash_fasta_lines(LinkedList *lines);
LinkedList *get_linked_fasta_lines(LinkedList *lines);

int RNA_splicing(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	LinkedList *input_lines = read_lines(input_file_pointer);
	char *prot_seq = remove_introns_and_translate(input_lines);
	printf("OR: %s\n", prot_seq);
	fprintf(output_file_pointer, "%s", prot_seq);
	fclose(output_file_pointer);
	fclose(input_file_pointer);
	return 0;
}

char *remove_introns_and_translate(LinkedList *input_lines) {
	LinkedList *fasta_lines = get_linked_fasta_lines(input_lines);
	char *main_strand = (char *)fasta_lines->root->value;
	char **intron_sequences = get_sorted_intron_sequences(fasta_lines);
	char *DNA = remove_introns(main_strand, intron_sequences, fasta_lines->size - 1);
	fasta_lines->print(fasta_lines);
	char *RNA = DNA_to_RNA(DNA);
	char *protein = RNA_to_protein(RNA);
	return protein;
}

/*load a fasta format file into a linked list of sequences*/
LinkedList *get_linked_fasta_lines(LinkedList *lines) {
	LinkedEntry *entry = lines->root;

	LinkedList *fasta_lines = new_linked_list('s');
	char *value = NULL;
	char *temp_value;
	int value_len = 0;
	while (entry->value != NULL) {
		char *line = (char *)entry->value;
		if (line[0] == '>') {
			if (value != NULL) {
				fasta_lines->add(fasta_lines, value);
			}
			value = NULL;
			value_len = 0;
		}
		else if (value == NULL) {
			value_len = strlen(line);
			temp_value = malloc(sizeof(char) * (value_len + 1));
			strcpy_s(temp_value, value_len + 1, line);
			value = temp_value;
			value[value_len] = '\0';
		}
		else {
			value_len += strlen(line);
			temp_value = malloc(sizeof(char) * (value_len + 1));
			strcpy_s(temp_value, value_len + 1, value);

			free(value);
			strcat_s(temp_value, value_len + 1, line);
			value = temp_value;
		}
		entry = entry->next;
	}
	fasta_lines->add(fasta_lines, value);
	return fasta_lines;
}

/*Load a fasta file into a hashtable keyed on sequence names*/
HashTable *get_hash_fasta_lines(LinkedList *lines) {
	LinkedEntry *entry = lines->root;
	HashTable *fasta_lines = new_hash_table(lines->size, 's');
	char *key = NULL;
	char *value = NULL;
	char *temp_value;
	int value_len = 0;
	while (entry->value != NULL) {
		char *line = (char *)entry->value;
		if (line[0] == '>') {
			if (value != NULL) {
				fasta_lines->add(fasta_lines, key, value);
			}
			key = line;
			value = NULL;
			value_len = 0;
		}
		else if (value == NULL) {
			value_len = strlen(line);
			temp_value = malloc(value_len + 1);
			strcpy_s(temp_value, value_len + 1, line);
			value = temp_value;
		}
		else{
			value_len += strlen(line);
			temp_value = malloc(value_len + 1);
			strcpy_s(temp_value, value_len, value);
			free(value);
			strcat_s(temp_value, value_len, line);
			value = temp_value;
		}
		entry = entry->next;
	}
	fasta_lines->add(fasta_lines, key, value);

	fasta_lines->print(fasta_lines);
	return fasta_lines;
}

/*For sorting strings long to small*/
int compare(const void * ca, const void * cb) {
	char **a = (char **)ca;
	char **b = (char **)cb;

	size_t fa = strlen(*a);
	size_t fb = strlen(*b);
	return fb - fa;
}

char **get_sorted_intron_sequences(LinkedList *all_sequences) {
	char **intron_sequences = (char **) all_sequences->to_array(all_sequences, 1, all_sequences->size);
	qsort(intron_sequences, all_sequences->size - 1, sizeof(char *), compare);
	return intron_sequences;
}

char *remove_introns(char *main_strand, char **intron_sequences, int no_seqs){
	int main_strand_size = strlen(main_strand);
	char *buffer = malloc(sizeof(char) * main_strand_size);
	int buffer_location = 0;

	for (int main_strand_index = 0; main_strand_index < main_strand_size; main_strand_index++){
		int failed;
		int intron_seq_index;
		for (int i = 0; intron_sequences[i]; i++){
			char *intron_seq = intron_sequences[i];
			failed = False;
			for (intron_seq_index = 0; intron_seq[intron_seq_index]; intron_seq_index++) {
				if (!main_strand[main_strand_index + intron_seq_index] || intron_seq[intron_seq_index] != main_strand[main_strand_index + intron_seq_index]) {
					failed = True;
					break;
				}
			}
			if (failed == False) {
				break;
			}
		}
		if (failed == False) {
			main_strand_index += intron_seq_index - 1;
		}
		else {
			buffer[buffer_location++] = main_strand[main_strand_index];
		}
	}
	buffer[buffer_location] = '\0';
	char *DNA = malloc(sizeof(char) * (buffer_location + 1));
	int t = strcpy_s(DNA, buffer_location + 1, buffer);
	free(buffer);
	return DNA;
}

char *DNA_to_RNA(char *DNA) {
	for (int index = 0; DNA[index]; index++) {

		if (DNA[index] == 'T') {
			DNA[index] = 'U';
		}
	}
	return DNA;
}

LinkedList *get_spliced_indices(char *sequence, char *sub_sequence) {
	int seq_index = 0;
	LinkedList *indices = new_linked_list('i');
	for (int sub_seq_index = 0; sub_sequence[sub_seq_index]; sub_seq_index++) {
		for (; sequence[seq_index]; seq_index++) {
			char sub_seq_char = sub_sequence[sub_seq_index];
			char seq_char = sequence[seq_index];
			if (sub_seq_char == seq_char) {
				int *location = (int *) malloc(sizeof(int));
				*location = seq_index + 1;
				indices->add(indices, location);
				seq_index++;
				break;
			}

		}
	}
	return indices;
}

int finding_a_spliced_motif(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	LinkedList *lines = read_lines(input_file_pointer);
	LinkedList *fasta_lines = get_linked_fasta_lines(lines);
	LinkedList *indices = get_spliced_indices(fasta_lines->root->value, fasta_lines->root->next->value);

	LinkedEntry *entry = indices->root;
	while (entry->next != NULL) {
		if (entry->next->next == NULL) {
			fprintf(output_file_pointer, "%d", *(int *)entry->value);
		}
		else {
			fprintf(output_file_pointer, "%d ", *(int *)entry->value);
		}
		entry = entry->next;
	}


	fclose(output_file_pointer);
	fclose(input_file_pointer);
	return 0;

}


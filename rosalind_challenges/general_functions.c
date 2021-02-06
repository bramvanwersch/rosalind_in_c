#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"

#define ERROR_BUFFER_SIZE 100

HashTable *amino_acid_table = NULL;


FILE* open_file(char *file_name, const char *mode) {
	// open a file with error checking
	errno_t error;
	FILE *file_pointer;
	if ((error = fopen_s(&file_pointer, file_name, mode)) != 0) {
		// When an error occurs print the error to standard error out
		char buf[ERROR_BUFFER_SIZE];
		strerror_s(buf, sizeof buf, error);
		fprintf_s(stderr, "cannot open file '%s': %s\n", file_name, buf);
		exit(1);
	}
	return file_pointer;
}

char* read_file(FILE *file_pointer) {
	// read a file into a null terminated string
	int long nr_bytes;
	char *buffer = 0;

	fseek(file_pointer, 0, SEEK_END);
	nr_bytes = ftell(file_pointer);
	fseek(file_pointer, 0, SEEK_SET);

	buffer = malloc(nr_bytes);
	if (buffer) {
		fread(buffer, 1, nr_bytes, file_pointer);
	}
	// make sure to null terminate the string
	buffer[nr_bytes] = '\0';
	return buffer;
}

LinkedList* read_lines(FILE *file_pointer) {
	// read all lines of a file into an array of null terminated strings
	int long nr_bytes;
	char *buffer = 0;
	int buffer_index = 0;

	int long line_byte_size;
	char ch;
	char *line;
	LinkedList* all_lines = new_linked_list('s');

	// allocate sufficient memory for any possible line --> might be overkill or realy bad, not sure
	fseek(file_pointer, 0, SEEK_END);
	nr_bytes = ftell(file_pointer);
	fseek(file_pointer, 0, SEEK_SET);
	if ((buffer = malloc(nr_bytes)) == NULL) {
		free(buffer);
		printf("malloc ran out of memory when allocating memory for the line buffer.");
		return NULL;
	}

	while ((ch = fgetc(file_pointer))) {
		if (ch == '\n' || ch == EOF) {
			buffer[buffer_index] = '\0';
			line_byte_size = sizeof(char) * (buffer_index + 1); // to include null pointer
			line = (char *)malloc(line_byte_size);
			if (line == NULL) {
				free(buffer);
				free(line);
				printf("malloc ran out of memory when allocating memory for the next line.\n");
				return NULL;
			}

			strcpy_s(line, buffer_index + 1, buffer);
			all_lines->append(all_lines, line);
			buffer_index = 0;
			if (ch == EOF) {
				break;
			}
		}
		else {
			buffer[buffer_index] = ch;
			buffer_index++;
		}
	}
	free(buffer);
	return all_lines;
}

/*load a fasta format file into a linked list of sequences*/
LinkedList *get_linked_fasta_lines(FILE *input_file_pointer) {

	// read lines
	LinkedList *lines = read_lines(input_file_pointer);
	LinkedEntry *entry = lines->root;

	// take the fasta lines and make sure to collect the DNA sequences over multiple newlines
	LinkedList *fasta_lines = new_linked_list('s');
	char *value = NULL;
	char *temp_value;
	int value_len = 0;
	while (entry->value != NULL) {
		char *line = (char *)entry->value;
		if (line[0] == '>') {
			if (value != NULL) {
				fasta_lines->append(fasta_lines, value);
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
	fasta_lines->append(fasta_lines, value);
	return fasta_lines;
}

char *DNA_to_RNA(char *DNA) {
	for (int index = 0; DNA[index]; index++) {

		if (DNA[index] == 'T') {
			DNA[index] = 'U';
		}
	}
	return DNA;
}


// create the amino acid table on the first call then subsequently ignore calls
HashTable *get_aa_table() {
	// setup the amino acid table
	if (amino_acid_table != NULL){
		return amino_acid_table;
	}
	amino_acid_table = new_hash_table('s');
	int sizeof_str = sizeof(char *);
	amino_acid_table->add(amino_acid_table, "GCU", "A", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GCC", "A", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GCA", "A", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GCG", "A", sizeof_str);

	amino_acid_table->add(amino_acid_table, "CGU", "R", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CGC", "R", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CGA", "R", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CGG", "R", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AGA", "R", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AGG", "R", sizeof_str);

	amino_acid_table->add(amino_acid_table, "AAU", "N", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AAC", "N", sizeof_str);

	amino_acid_table->add(amino_acid_table, "GAU", "D", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GAC", "D", sizeof_str);

	amino_acid_table->add(amino_acid_table, "UGU", "C", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UGC", "C", sizeof_str);

	amino_acid_table->add(amino_acid_table, "CAA", "Q", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CAG", "Q", sizeof_str);

	amino_acid_table->add(amino_acid_table, "GAA", "E", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GAG", "E", sizeof_str);

	amino_acid_table->add(amino_acid_table, "GGU", "G", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GGC", "G", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GGA", "G", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GGG", "G", sizeof_str);

	amino_acid_table->add(amino_acid_table, "CAU", "H", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CAC", "H", sizeof_str);

	amino_acid_table->add(amino_acid_table, "AUG", "M", sizeof_str);

	amino_acid_table->add(amino_acid_table, "AUU", "I", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AUC", "I", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AUA", "I", sizeof_str);

	amino_acid_table->add(amino_acid_table, "CUU", "L", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CUC", "L", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CUA", "L", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CUG", "L", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UUA", "L", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UUG", "L", sizeof_str);

	amino_acid_table->add(amino_acid_table, "AAA", "K", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AAG", "K", sizeof_str);

	amino_acid_table->add(amino_acid_table, "UUU", "F", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UUC", "F", sizeof_str);

	amino_acid_table->add(amino_acid_table, "CCU", "P", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CCC", "P", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CCA", "P", sizeof_str);
	amino_acid_table->add(amino_acid_table, "CCG", "P", sizeof_str);

	amino_acid_table->add(amino_acid_table, "UCU", "S", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UCC", "S", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UCA", "S", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UCG", "S", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AGU", "S", sizeof_str);
	amino_acid_table->add(amino_acid_table, "AGC", "S", sizeof_str);

	amino_acid_table->add(amino_acid_table, "ACU", "T", sizeof_str);
	amino_acid_table->add(amino_acid_table, "ACC", "T", sizeof_str);
	amino_acid_table->add(amino_acid_table, "ACA", "T", sizeof_str);
	amino_acid_table->add(amino_acid_table, "ACG", "T", sizeof_str);

	amino_acid_table->add(amino_acid_table, "UGG", "W", sizeof_str);

	amino_acid_table->add(amino_acid_table, "UAU", "Y", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UAC", "Y", sizeof_str);

	amino_acid_table->add(amino_acid_table, "GUU", "V", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GUC", "V", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GUA", "V", sizeof_str);
	amino_acid_table->add(amino_acid_table, "GUG", "V", sizeof_str);

	// the 3 stop codons
	amino_acid_table->add(amino_acid_table, "UAA", "X", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UGA", "X", sizeof_str);
	amino_acid_table->add(amino_acid_table, "UAG", "X", sizeof_str);
	return amino_acid_table;
}

char *RNA_to_protein(char *rna_string) {
	HashTable *amino_acid_table = get_aa_table();
	char ch;
	char codon[4];
	int total_aas = strlen(rna_string) / 3;
	char *protein_strand;

	protein_strand = (char *)malloc(sizeof(char) * (total_aas + 1)); // to include the '\0'

	int codon_index = 0;
	int protein_stand_index = 0;
	while (ch = *rna_string++) {
		codon[codon_index] = ch;
		codon_index++;
		if (codon_index == 3) {
			codon[codon_index] = '\0';
			char *value = amino_acid_table->get(amino_acid_table, codon);
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

char* reverse_complement_DNA(char *sequence) {
	char ch;
	int string_len;
	string_len = strlen(sequence);

	char *reverse_complement = malloc(sizeof(char) * (string_len + 1));

	// move over the string in reverse and put the complement characters in the output_file
	int rev_comp_index = 0;
	while (--string_len >= 0) {
		ch = sequence[string_len];
		switch (ch) {
		case 'A':
			reverse_complement[rev_comp_index] = 'T';
			break;
		case 'C':
			reverse_complement[rev_comp_index] = 'G';
			break;
		case 'G':
			reverse_complement[rev_comp_index] = 'C';
			break;
		case 'T':
			reverse_complement[rev_comp_index] = 'A';
			break;
		default:
			fprintf(stderr, "Invalid character %d\n", ch);
			exit(1);
		}
		rev_comp_index++;
	}
	reverse_complement[rev_comp_index] = '\0';
	return reverse_complement;
}
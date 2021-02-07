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
			start_locations->append(start_locations, motif_location, sizeof(char *));
		}
	}
	return start_locations;
}

char *remove_introns(char *main_strand, char **intron_sequences, int no_seqs);
char **get_sorted_intron_sequences(LinkedList *all_sequences);
char *remove_introns_and_translate(LinkedList *input_lines);

int RNA_splicing(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	LinkedList *fasta_lines = get_linked_fasta_lines(input_file_pointer);
	char *prot_seq = remove_introns_and_translate(fasta_lines);
	printf("OR: %s\n", prot_seq);
	fprintf(output_file_pointer, "%s", prot_seq);
	fclose(output_file_pointer);
	fclose(input_file_pointer);
	return 0;
}

char *remove_introns_and_translate(LinkedList *fasta_lines) {
	char *main_strand = (char *)fasta_lines->root->value;
	char **intron_sequences = get_sorted_intron_sequences(fasta_lines);
	char *DNA = remove_introns(main_strand, intron_sequences, fasta_lines->size - 1);
	fasta_lines->print(fasta_lines);
	char *RNA = DNA_to_RNA(DNA);
	char *protein = RNA_to_protein(RNA);
	return protein;
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
				indices->append(indices, location, sizeof(char *));
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
	LinkedList *fasta_lines = get_linked_fasta_lines(input_file_pointer);
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

LinkedList *find_restriction_sites(char *sequence);

int locating_restriction_sites(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	
	LinkedList *fasta_lines = get_linked_fasta_lines(input_file_pointer);
	char *sequence = fasta_lines->root->value;
	LinkedList *result = find_restriction_sites(sequence);
	LinkedEntry *entry = result->root;

	while (entry->next != NULL) {
		int *value = entry->value;
		if (entry->next->next == NULL) {
			fprintf(output_file_pointer, "%d %d", value[0], value[1]);
		}
		else {
			fprintf(output_file_pointer, "%d %d\n", value[0], value[1]);
		}
		printf("%d %d\n", value[0], value[1]);
		entry = entry->next;
	}

	fclose(output_file_pointer);
	fclose(input_file_pointer);
	return 0;
}

LinkedList *find_restriction_sites(char *sequence) {
	LinkedList *result = new_linked_list('p');
	char base;
	int seq_len = strlen(sequence);
	for (int index = 0; index < seq_len; index++) {
		base = sequence[index];
		//find palindromes in a 4 to 12 range
		for (int palindrome_range = 4; palindrome_range <= MIN(seq_len - index , 12); palindrome_range += 2) {
			int succes = True;
			for (int offset = 0; offset < palindrome_range / 2; offset++) {
				// test if A vs T or C vs G
				if (abs(sequence[index + offset] - sequence[index + palindrome_range - 1 - offset]) == 19 || 
					abs(sequence[index + offset] - sequence[index + palindrome_range - 1 - offset]) == 4) {
					continue;
				}
				else {
					succes = False;
					break;
				}
			}
			if (succes == True) {
				int *r_palindrome = malloc(sizeof(int) * 2);
				r_palindrome[0] = index + 1;
				r_palindrome[1] = palindrome_range;
				result->append(result, r_palindrome, sizeof(char *));
			}
		}
	}
	return result;
}

Set *extract_open_reading_frames(char *sequence);

int open_reading_frames(char *argv[]) {
	// read the input file into memory
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");

	LinkedList *fasta_lines = get_linked_fasta_lines(input_file_pointer);
	char *sequence = fasta_lines->root->value;
	char *r_sequence = reverse_complement_DNA(sequence);
	Set *forward_orfs = extract_open_reading_frames(sequence);

	Set *reverse_orfs = extract_open_reading_frames(r_sequence);

	forward_orfs->union_sets(forward_orfs, reverse_orfs);

	forward_orfs->print(forward_orfs);
	char **forward_orfs_array = forward_orfs->values(forward_orfs);
	for (int index = 0; index < forward_orfs->current_size - 1; index++) {
		char *orf = forward_orfs_array[index];
		fprintf(output_file_pointer, "%s\n", orf);
	}
	fprintf(output_file_pointer, "%s", forward_orfs_array[forward_orfs->current_size - 1]);

	fclose(output_file_pointer);
	fclose(input_file_pointer);
	return 0;
}

Set *extract_open_reading_frames(char *sequence) {
	HashTable *amino_acid_table = get_aa_table();
	Set *orfs = new_set();
	int seq_len = strlen(sequence);
	char codon[4];
	int reading_frame;
	LinkedList *current_orfs[3] = {NULL, NULL, NULL};

	for (int index = 0; index < seq_len - 3; index++) {
		for (int codon_index = 0; codon_index < 3; codon_index++) {
			char nuc = sequence[index + codon_index];
			if (nuc == 'T') {
				nuc = 'U';
			}
			codon[codon_index] = nuc;
		}
		codon[3] = '\0';
		char *aa = amino_acid_table->get(amino_acid_table, codon);
		reading_frame = index % 3;
		if (current_orfs[reading_frame] == NULL) {
			if (aa[0] == 'M') {
				current_orfs[reading_frame] = new_linked_list('s');
				current_orfs[reading_frame]->append(current_orfs[reading_frame], &aa[0], sizeof(char));
			}
		}
		else{
			// save orf and all orfs contained in that orf into the orfs linked list
			if (aa[0] == 'X') {
				int orf_bit_size = sizeof(char) * (current_orfs[reading_frame]->size + 1);
				char *orf = (char *)malloc(orf_bit_size);

				LinkedEntry *entry = current_orfs[reading_frame]->root;
				int index = 0;
				LinkedList *internal_indexes = new_linked_list('d');
				while (entry->next != NULL) {
					char internal_aa = *(char *)entry->value;
					// catch orfs inside other orfs
					if (internal_aa == 'M' && index != 0) {
						int *c_index = (int *)malloc(sizeof(int *));
						*c_index = index;
						internal_indexes->append(internal_indexes, c_index, sizeof(int));
						free(c_index);
					}
					orf[index] = internal_aa;
					entry = entry->next;
					index++;
				}
				orf[index] = '\0';
				orfs->add(orfs, orf);
				entry = internal_indexes->root;
				
				// take the internal starting points and make new strings
				while (entry->next != NULL) {
					int start_index = *(int *)entry->value;
					int buffer_size = (current_orfs[reading_frame]->size - start_index + 1);
					int buffer_bit_size = sizeof(char) * buffer_size;
					char *orf_buffer = (char*)malloc(buffer_bit_size);
					memcpy(orf_buffer, &orf[start_index], buffer_size);
					orfs->add(orfs, orf_buffer);
					free(orf_buffer);
					entry = entry->next;
				}
				// free the memory again
				free(orf);
				current_orfs[reading_frame]->delete(current_orfs[reading_frame]);
				current_orfs[reading_frame] = NULL;
				internal_indexes->delete(internal_indexes);
				internal_indexes = NULL;
			}
			else {
				// add the next amino acid
				current_orfs[reading_frame]->append(current_orfs[reading_frame], &aa[0], sizeof(char *));
			}
		}
	}
	return orfs;
}


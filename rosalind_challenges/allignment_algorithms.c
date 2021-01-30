#include <stdio.h>
#include <stdlib.h>

#include "allignment_algorithms.h"
#include "general_functions.h"

FILE *input_file_pointer;
FILE *output_file_pointer;

int counting_point_mutations(char *argv[]){
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	LinkedList *lines = read_lines(input_file_pointer);
	LinkedEntry *entry_line = lines->root;
	char *line1 = (char *) entry_line->value;
	char *line2 = (char *) entry_line->next->value;
	int differences = count_line_differences(line1, line2);
	printf("Total differences %d\n", differences);
	fprintf(output_file_pointer, "%d", differences);
	fclose(input_file_pointer);
	fclose(output_file_pointer);
	return 0;
}

int count_line_differences(char *line1, char *line2) {
	char ch1, ch2;
	int difference_count = 0;

	while (ch1 = *line1++) {
		ch2 = *line2++;
		if (ch1 != ch2) {
			difference_count++;
		}
	}
	return difference_count;
}

int transitions_and_transversions(char *argv[]) {
	input_file_pointer = open_file(argv[2], "r");
	output_file_pointer = open_file("output.txt", "w");
	LinkedList *lines = get_linked_fasta_lines(input_file_pointer);
	LinkedEntry *entry_line = lines->root;
	
	char *line1 = (char *)entry_line->value;
	char *line2 = (char *)entry_line->next->value;
	int *transitions_transversions = count_transitions_transversions(line1, line2);
	double ratio = (double)transitions_transversions[0] / (double)transitions_transversions[1];
	printf("%f\n", ratio);
	fprintf(output_file_pointer, "%f", ratio);
	fclose(input_file_pointer);
	fclose(output_file_pointer);
	return 0;
}

int *count_transitions_transversions(char *line1, char *line2) {
	char ch1, ch2;
	int *transitions_transversions = malloc(sizeof(int) * 2);
	transitions_transversions[0] = 0;
	transitions_transversions[1] = 0;

	while (ch1 = *line1++) {
		ch2 = *line2++;
		if (ch1 != ch2) {
			// transition A-G
			if (abs(ch1 - ch2) == 6) {
				transitions_transversions[0]++;
			}
			// transition C-T
			else if (abs(ch1 - ch2) == 17) {
				transitions_transversions[0]++;
			}
			else {
				transitions_transversions[1]++;
			}

		}
	}
	return transitions_transversions;
}
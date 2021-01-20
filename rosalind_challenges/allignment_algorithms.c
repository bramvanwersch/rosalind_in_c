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
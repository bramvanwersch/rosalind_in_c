#include <stdio.h>
#include <string.h>
#include "string_algorithms.h"
#include "allignment_algorithms.h"
#include "combinatorics_algorithms.h"


char *argv[3];
char input_name_buffer[100];
char output_name_buffer[100];

char *failed_test_names[50];
int failed_test_count = 0;
int total_test_count = 0;

static int check_output(char *expected_out_file) {
	FILE *output_file_pointer = open_file("output.txt", "r");
	FILE *expected_output_file_pointer = open_file(expected_out_file, "r");
	char ch1, ch2;
	int char_index = 0;
	int succes = False;
	while (True) {
		ch1 = fgetc(output_file_pointer);
		ch2 = fgetc(expected_output_file_pointer);
		if (ch1 == EOF || ch2 == EOF) {
			if (ch1 != ch2) {
				fprintf(stderr, "Lenght of expected and actual output do not match.\n");
				break;
			}
			printf("Output is as expected.\n");
			succes = True;
			break;
		}
		else if (ch2 != ch1) {
			fprintf(stderr, "Expected and actual output dont match at index %d\n", char_index);
			break;
		}
		char_index++;
	}
	fclose(output_file_pointer);
	fclose(expected_output_file_pointer);
	return succes;
}

static void run_test(char *challenge_name, int(*function)(char **argv)) {
	int challenge_name_len = strlen(challenge_name);
	int in_buf_size = challenge_name_len + 14; // 13 for extras 1 for eof
	int out_buf_size = challenge_name_len + 15; // 14 for extras 1 for eof
	snprintf(input_name_buffer, in_buf_size, "./inputs/%s.txt", challenge_name);
	snprintf(output_name_buffer, out_buf_size, "./outputs/%s.txt", challenge_name);
	argv[2] = input_name_buffer;
	printf("Running test for challenge '%s':\n", challenge_name);
	function(argv);
	int return_code = check_output(output_name_buffer);
	if (return_code == True) {
		printf("SUCCES!\n\n");
	}
	else {
		failed_test_names[failed_test_count] = challenge_name;
		failed_test_count++;
		printf("FAILED!\n\n");
	}
	total_test_count++;
}

static void print_test_report() {
	printf("-------------\nFINAL REPORT:\n-------------\n");
	printf("%d/%d tests where succesfull\n", total_test_count - failed_test_count, total_test_count);
	if (total_test_count > 0) {
		printf("\nThe following tests FAILED:\n");
		for (int i = 0; i < failed_test_count; i++) {
			printf("- %s\n", failed_test_names[i]);
		}
	}
}

int main_test_solutions()
{
	printf("Running tests for all the challenges:\n\n");
	// first 2 arguments dont matter anymore.
	argv[0] = "empty";
	argv[1] = "empty";

	run_test("counting_DNA_nucleotides", counting_DNA_nucleotides);

	run_test("transcribing_DNA_into_RNA", transcribing_DNA_into_RNA);

	run_test("complementing_a_strand_of_DNA", complementing_a_strand_of_DNA);

	run_test("computing_GC_content", computing_GC_content);

	run_test("counting_point_mutations", counting_point_mutations);

	run_test("translating_RNA_into_protein", translating_RNA_into_protein);
	
	run_test("finding_a_motif_in_DNA", finding_a_motif_in_DNA);
	
	run_test("inferring_mRNA_from_protein", inferring_mRNA_from_protein);

	run_test("open_reading_frames", open_reading_frames);

	run_test("locating_restriction_sites", locating_restriction_sites);
	
	run_test("RNA_splicing", RNA_splicing);
	
	run_test("finding_a_spliced_motif", finding_a_spliced_motif);
	
	run_test("transitions_and_transversions", transitions_and_transversions);
	

	print_test_report();
	return 0;
}
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"

#define ERROR_BUFFER_SIZE 100


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
	LinkedList* all_lines = new_linked_list();

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
			all_lines->add(all_lines, line);
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

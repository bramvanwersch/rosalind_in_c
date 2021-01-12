#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"

#define ERROR_BUFFER_SIZE 100

FILE *open_file(char *file_name, const char *mode) {
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
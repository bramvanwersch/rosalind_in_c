#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"

#define ERROR_BUFFER_SIZE 100

FILE *open_file(char *file_name, const char *mode) {
	errno_t error;
	FILE *input_file_pointer;
	if ((error = fopen_s(&input_file_pointer, file_name, mode)) != 0) {
		// When an error occurs print the error to standard error out
		char buf[ERROR_BUFFER_SIZE];
		strerror_s(buf, sizeof buf, error);
		fprintf_s(stderr, "cannot open file '%s': %s\n", file_name, buf);
		exit(1);
	}
	return input_file_pointer;
}
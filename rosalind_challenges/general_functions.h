#pragma once
#include <stdio.h>

typedef struct LinkedList {
	struct LinkedList *next;
	char *value;
} LinkedList;

FILE *open_file(char *file_name, const char *mode);
char* read_file(FILE *file_pointer);
LinkedList*  read_lines(FILE *file_pointer);

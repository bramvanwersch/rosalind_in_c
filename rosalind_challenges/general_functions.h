#pragma once
#include <stdio.h>
#include "data_structures.h"

#define True 1
#define False 0

FILE *open_file(char *file_name, const char *mode);
char *read_file(FILE *file_pointer);
LinkedList *read_lines(FILE *file_pointer);
LinkedList *get_linked_fasta_lines(FILE *lines);

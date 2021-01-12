#pragma once
#include <stdio.h>

FILE *open_file(char *file_name, const char *mode);
char* read_file(FILE *file_pointer);

#pragma once
#include <stdio.h>

/*Linked list values*/
typedef struct LinkedList {
	struct LinkedList *next;
	char *value;
} LinkedList;

/*Hash table values*/

/*hashtable structures*/
typedef struct Entry {
	struct Entry *next;
	char *key;
	char *value;
} Entry;

typedef struct HashTable {
	int max_size;
	int current_size;
	struct Entry **table;
	Entry *(*get)(struct HashTable *self, char *key);
	Entry *(*add)(struct HashTable *self, char *key, char *value);
	int(*delete)(struct HashTable *self, char *key);
	void(*print)(struct HashTable *self);
} HashTable;

int main_hash();
HashTable *new_hash_table(int size);

FILE *open_file(char *file_name, const char *mode);
char *read_file(FILE *file_pointer);
LinkedList *read_lines(FILE *file_pointer);

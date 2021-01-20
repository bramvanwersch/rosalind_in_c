#pragma once
#include <stdio.h>
#define True 1
#define False 0
#define INT_TYPE = 0;
#define STRING_TYPE = 1;

typedef struct LinkedEntry {
	struct LinkedEntry *next;
	void *value;
}LinkedEntry;

/*Linked list values*/
typedef struct LinkedList {
	struct LinkedEntry *root;
	struct LinkedEntry *end;
	LinkedEntry *(*next)(struct LinkedList *self, void *value);
}LinkedList;

LinkedList* new_linked_list();

/*hashtable structures*/
typedef struct HashEntry {
	struct HashEntry *next;
	char *key;
	char *value;
} HashEntry;

typedef struct HashTable {
	int max_size;
	int current_size;
	struct HashEntry **table;
	char *(*get)(struct HashTable *self, char *key);
	HashEntry *(*add)(struct HashTable *self, char *key, char *value);
	int(*delete)(struct HashTable *self, char *key);
	void(*print)(struct HashTable *self);
} HashTable;

HashTable *new_hash_table(int size);

FILE *open_file(char *file_name, const char *mode);
char *read_file(FILE *file_pointer);
LinkedList *read_lines(FILE *file_pointer);

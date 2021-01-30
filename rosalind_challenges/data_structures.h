#pragma once
#include <stdio.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


/*Linked list structures*/
typedef struct LinkedEntry {
	struct LinkedEntry *next;
	void *value;
}LinkedEntry;

typedef struct LinkedList {
	struct LinkedEntry *root;
	struct LinkedEntry *end;
	LinkedEntry *(*add)(struct LinkedList *self, void *value);
	void **(*to_array)(struct LinkedList *self, size_t from, size_t until);
	size_t size;
	void (*print)(struct LinkedList *self, char type);
}LinkedList;

LinkedList* new_linked_list();
int test_linked_list();

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

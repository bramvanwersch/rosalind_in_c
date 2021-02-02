#pragma once
#include <stdio.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define True 1
#define False 0


/*Linked list structures*/
typedef struct LinkedEntry {
	struct LinkedEntry *next;
	void *value;
}LinkedEntry;

typedef struct LinkedList {
	char type;
	struct LinkedEntry *root;
	struct LinkedEntry *end;
	LinkedEntry *(*append)(struct LinkedList *self, void *value);
	void **(*to_array)(struct LinkedList *self, size_t from, size_t until);
	size_t size;
	void (*print)(struct LinkedList *self);
	void (*delete)(struct LinkedList *self);
}LinkedList;

LinkedList* new_linked_list(char type);
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
	char type;
	struct HashEntry **table;
	void *(*get)(struct HashTable *self, char *key);
	HashEntry *(*add)(struct HashTable *self, char *key, void *value);
	int(*delete)(struct HashTable *self, char *key);
	void(*print)(struct HashTable *self);
} HashTable;

HashTable *new_hash_table(int size, char type);
int test_hash_table();

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
	void (*append)(struct LinkedList *self, void *value);
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
	HashEntry **table;
	void *(*get)(struct HashTable *self, char *key);
	void(*add)(struct HashTable *self, char *key, void *value);
	void(*remove)(struct HashTable *self, char *key);
	void(*print)(struct HashTable *self);
	int(*in)(struct HashTable *self, char *key);
	char **(*keys)(struct HashTable *self);
} HashTable;

HashTable *new_hash_table(int size, char type);
int test_hash_table();


/*Set structures*/
typedef struct SetEntry{
	struct SetEntry *next;
	char *value;
}SetEntry;

typedef struct Set {
	int current_size;
	int max_size;
	SetEntry **table;
	void(*add)(struct Set *self, char *value);
	void(*remove)(struct Set *self, char *value);
	void(*print)(struct Set *self);
	int(*in)(struct Set *self, char *value);
	char **(*values)(struct Set *self);

}Set;

Set *new_set(int size);
int test_set();
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
	void (*append)(struct LinkedList *self, void *value, int sizeof_value);
	void **(*to_array)(struct LinkedList *self, int from, int until);
	int size;
	void (*print)(struct LinkedList *self);
	void (*delete)(struct LinkedList *self);
}LinkedList;

LinkedList* new_linked_list(char type);
int test_linked_list();


/*hashtable structures*/
typedef struct HashEntry {
	struct HashEntry *next;
	char *key;
	void *value;
} HashEntry;

typedef struct HashTable {
	int max_size;
	int current_size;
	char type;
	HashEntry **table;
	void *(*get)(struct HashTable *self, char *key);
	void(*add)(struct HashTable *self, char *key, void *value,
		int sizeof_value);
	void(*remove)(struct HashTable *self, char *key);
	void(*print)(struct HashTable *self);
	int(*in)(struct HashTable *self, char *key);
	char **(*keys)(struct HashTable *self);
} HashTable;

HashTable *new_hash_table(char type);
int test_hash_table();


/*Set structures*/
typedef struct SetEntry{
	struct SetEntry *next;
	char *key;
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
	void(*union_sets)(struct Set *self, struct Set *other_set);

}Set;

Set *new_set();
int test_set();
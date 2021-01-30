#include <stdio.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

#define ERROR_BUFFER_SIZE 100

// GENERAL USE FUNCTIONS
void print_type(char type, void *value) {
	switch (type) {
	case ('s'):
		printf("%s", (char *)value);
		break;
	case ('d'):
		printf("%d", *(int *)value);
		break;
	case ('f'):
		printf("%f", *(double *)value);
		break;
	case ('p'):
		printf("%p", value);
		break;
	default:
		printf("Unrecognized type %c", type);
		break;
	}
}

//LINKED LIST FUNCTIONS

LinkedEntry *add_entry(LinkedList *self, void *value);
void **to_array(LinkedList *self, size_t from, size_t until);
void print_linked_list(LinkedList *self);

LinkedList *new_linked_list(char type) {
	LinkedList *new_list = (LinkedList *)malloc(sizeof(LinkedList));
	LinkedEntry *root_entry = (LinkedEntry *)malloc(sizeof(LinkedEntry));

	root_entry->value = NULL;
	root_entry->next = NULL;

	new_list->root = root_entry;
	new_list->type = type;
	new_list->end = root_entry;
	new_list->add = add_entry;
	new_list->to_array = to_array;
	new_list->size = 0;
	new_list->print = print_linked_list;
	return new_list;
}

LinkedEntry *add_entry(LinkedList *self, void *value) {
	LinkedEntry* new_entry = (LinkedEntry *)malloc(sizeof(LinkedEntry));
	self->end->value = value;
	self->end->next = new_entry;
	new_entry->next = NULL;
	new_entry->value = NULL;
	self->end = new_entry;
	self->size++;
	return new_entry;
}

void **to_array(LinkedList *self, size_t from, size_t until) {
	if (until <= from) {
		printf("Error: From (%d) must be smaller then until (%d).\n", from , until);
		exit(1);
	}
	from = MAX(0, from);
	until = MIN(self->size, until);
	void **out_array = malloc(sizeof(void *) * ((until - from) + 1));
	if (out_array == NULL) {
		printf("Error. Could not allocate memory for array.\n");
		exit(1);
	}
	LinkedEntry *entry = self->root;
	size_t index = 0;
	size_t out_index = 0;
	while (entry->next != NULL) {
		if (index < from) {
		}
		else if (index > until) {
			break;
		}
		else {
			out_array[out_index++] = entry->value;
		}
		entry = entry->next;
		index++;
	}
	out_array[out_index] = NULL;

	return out_array;
}

void print_linked_list(LinkedList *self) {
	// couple of possible types s = string, d = int, f = double, p = pointers --> works for any value.
	LinkedEntry *entry = self->root;
	printf("[");
	while (entry->next != NULL) {
		if (entry->value == NULL) {
			printf("NULL");
		}
		else {
			print_type(self->type, entry->value);
		}
		if (entry->next->next != NULL) {
			printf(", ");
		}

		entry = entry->next;
	}
	printf("]\n");
}

// HASHTABLE FUNCTIONS
unsigned hash(char *key, int size);
char *get(HashTable *table, char *key);
HashEntry *in(HashTable *self, char *key);
HashEntry *add_key(HashTable *table, char *key, void *value);
void increase_table_size(HashTable *table);
int delete(HashTable *self, char *key);
void free_entry(HashEntry *e);
void print_hash_table(HashTable *self);
void print_hash_entry(HashEntry *e, int index);

/*Create a new hash table*/
HashTable *new_hash_table(int size, char type) {
	int i;
	HashTable *new_table;

	//make sure the table is bigger then 0
	if (size < 1) {
		printf("Cannot instantiate table smaller then 1.");
		exit(1);
	}

	//allocate the hash table with check
	if ((new_table = malloc(sizeof(HashTable))) == NULL) {
		printf("Cannot allocate memory for the new HashTable\n");
		exit(1);
	}
	// Allocate pointers to the head nodes.
	if ((new_table->table = malloc(sizeof(HashEntry *) * size)) == NULL) {
		printf("Cannot allocate memory for the new HashTable table\n");
		exit(1);
	}
	for (i = 0; i < size; i++) {
		new_table->table[i] = NULL;
	}

	new_table->max_size = size;
	new_table->type = type;
	//asign the function pointers
	new_table->add = add_key;
	new_table->get = get;
	new_table->print = print_hash_table;
	new_table->delete = delete;
	return new_table;
}

/*djb2 --> http://www.cse.yorku.ca/~oz/hash.html tested to be a very decent and simple hash function*/
unsigned hash(char *key, int size)
{
	unsigned long hash = 5381;
	int c;

	while (c = *key++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return ((unsigned)hash % size);
}

/*Get a value corresponding to a key from the hashtable*/
char *get(HashTable *self, char *key) {
	HashEntry *result = in(self, key);
	if (result == NULL) {
		printf("Keyerror. Key '%s' not in hashtable", key);
		exit(1);
	}
	return result->value;
}

HashEntry *in(HashTable *self, char *key) {
	struct HashEntry *entry_pointer;

	//Look for an entry as long as there are more entries, in case of collisions
	for (entry_pointer = self->table[hash(key, self->max_size)]; entry_pointer != NULL; entry_pointer = entry_pointer->next) {
		//check if the name of the pointer is that of the key
		if (strcmp(key, entry_pointer->key) == 0) {
			return entry_pointer;
		}
	}
	//no key found
	return NULL;
}

/*Add a key to the hashtable or replace the old values*/
HashEntry *add_key(HashTable *self, char *key, void *value) {
	HashEntry *entry_pointer;
	unsigned hash_value;
	self->current_size++;

	// when the dictionary becomes to big collision becomes more likely, so increase the size
	if (self->current_size > (int)(self->max_size * 0.8)) {
		increase_table_size(self);

	}

	//no key found with name of key
	if ((entry_pointer = in(self, key)) == NULL) {
		entry_pointer = (HashEntry *)malloc(sizeof(*entry_pointer));
		//check if there is a problem allocating memory 
		if (entry_pointer == NULL || ((entry_pointer->key = _strdup(key)) == NULL)) {
			printf("Cannot allocate memory for the new Entry\n");
			exit(1);
		}

		//assign the current entry to the next and the new one to the current
		hash_value = hash(key, self->max_size);
		entry_pointer->next = self->table[hash_value];
		self->table[hash_value] = entry_pointer;
	}
	else {
		//free the memory for a new value
		free(entry_pointer->value);
	}
	//place the value in the table and check for storage trouble
	if ((entry_pointer->value = _strdup(value)) == NULL) {
		printf("Not enough memory to place new entry in table\n");
		exit(1);

	}
	return entry_pointer;
}

void increase_table_size(HashTable *hash_table) {
	int orig_size = hash_table->max_size;
	HashEntry **orig_table = hash_table->table;
	hash_table->max_size *= 2;
	hash_table->current_size = 0;

	// reallocate the table
	if ((hash_table->table = malloc(sizeof(HashEntry *) * hash_table->max_size)) == NULL) {
		printf("Cannot allocate memory for new hash table\n");
		exit(1);
	}

	for (int index = 0; index < hash_table->max_size; index++) {
		hash_table->table[index] = NULL;
	}

	// copy keys
	for (int index = 0; index < orig_size; index++) {
		HashEntry *entry = orig_table[index];
		while (entry != NULL) {
			hash_table->add(hash_table, entry->key, entry->value);
			entry = entry->next;
		}
	}
	free(orig_table);
}

/* Delete a key and value from the hashtable. Return 0 on succes and 1 on failure */
int delete(HashTable *self, char *key) {
	HashEntry *ep1, *ep2;
	self->current_size--;

	/* create 2 pointers, 1 to the current and one to the previous element */
	for (ep1 = ep2 = self->table[hash(key, self->max_size)]; ep1 != NULL; ep2 = ep1, ep1 = ep1->next) {
		if (strcmp(key, ep1->key) == 0) {

			/* when the key is at the first level  */
			if (ep1 == ep2) {
				self->table[hash(key, self->max_size)] = ep1->next;
			}
			else {
				ep2->next = ep1->next;
			}

			/*  Free memory  */
			free_entry(ep1);
			return 0;
		}
	}
	/* No key found */
	return 1;
}

void free_entry(HashEntry *e) {
	free(e->value);
	free(e->key);
	free(e);
}

/*Function for printing the hashtable*/
void print_hash_table(HashTable *self) {
	int i;

	printf("Hastable of size %d:{\n", self->max_size);
	for (i = 0; i < self->max_size; i++) {
		HashEntry *entry_pointer = self->table[i];
		if (entry_pointer != NULL) {
			print_hash_entry(entry_pointer, i);
		}
	}
	printf("}\n");
}

/*Recursive function for printing entries of the hashtable*/
void print_hash_entry(HashEntry *e, int index) {
	//if the index is bigger it is a genuine index. Else it is an indicator that is a repeat.
	if (index >= 0) {
		printf("\tEntry %d, %s: %s", index, e->key, e->value);
	}
	else {
		printf(", %s: %s", e->key, e->value);
	}
	if (e->next != NULL) {
		print_hash_entry(e->next, -1);
	}
	else {
		printf("\n");
	}
}

// TESTING FUNCTIONS

int test_linked_list() {
	LinkedList *test = new_linked_list('s');
	test->print(test);
	test->add(test, "test1");
	test->print(test);
	test->to_array(test, 0, test->size);
	test->add(test, "test2");
	test->add(test, "test3");
	test->add(test, "test4");
	test->print(test);

	LinkedList *test2 = new_linked_list('d');
	int k = 1;
	int j = 5;
	test2->add(test2, &k);
	test2->add(test2, &j);
	test2->print(test2);
	test->to_array(test, 0, test->size);

	return 0;
}

int test_hash_table() {
	HashTable *test_table = new_hash_table(4, 's');
	test_table->print(test_table);
	test_table->add(test_table, "key1", "value1");
	return 0;
}
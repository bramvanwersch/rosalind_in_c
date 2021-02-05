#include <stdio.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

#define ERROR_BUFFER_SIZE 100
#define MAX_HASH_TABLE_RACTION_FILL 0.8
#define MIN_HASH_TABLE_FRACTION_FILL 0.3 // this value should be chosen in sutch a way that shrinking does not cause subsequent growing of the table

// GENERAL USE FUNCTIONS

void print_type(char type, void *value);
void raise_memory_error(char *message);

void print_type(char type, void *value) {
	switch (type) {
	case ('s'):
		printf("%s", (char *)value);
		break;
	case ('c'):
		printf("%c", *(char *)value);
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

/*Signify the user there is not enough memory and exit*/
void raise_memory_error(char *message) {
	printf("MemoryError: Cannot allocate enought memory. %s", message);
	exit(1);
}

//LINKED LIST FUNCTIONS

void append_entry(LinkedList *self, void *value);
void **to_array(LinkedList *self, size_t from, size_t until);
void print_linked_list(LinkedList *self);
void delete_linked_list(LinkedList *self);

/*
Create a new LinkedList

type (char): type used for certain type dependant operations. Type checking will never occur
*/
LinkedList *new_linked_list(char type) {
	LinkedList *new_list;
	new_list = malloc(sizeof(*new_list));
	if (new_list == NULL) {
		raise_memory_error("LinkedList creation failed.");
	}
	LinkedEntry *root_entry;
	root_entry = (LinkedEntry *)malloc(sizeof(*root_entry));
	if (root_entry == NULL) {
		raise_memory_error("LinkedList creation failed.");
	}

	root_entry->value = NULL;
	root_entry->next = NULL;

	// values
	new_list->type = type;
	new_list->root = root_entry;
	new_list->end = root_entry;
	new_list->size = 0;

	// functions
	new_list->append = append_entry;
	new_list->to_array = to_array;
	new_list->print = print_linked_list;
	new_list->delete = delete_linked_list;
	return new_list;
}

void append_entry(LinkedList *self, void *value) {
	LinkedEntry* new_entry = (LinkedEntry *)malloc(sizeof(LinkedEntry));
	self->end->value = value;
	self->end->next = new_entry;
	new_entry->next = NULL;
	new_entry->value = NULL;
	self->end = new_entry;
	self->size++;
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

void delete_linked_list(LinkedList *self) {
	LinkedEntry *entry = self->root;
	LinkedEntry *old_entry;
	while (entry->next != NULL) {
		old_entry = entry;
		entry = entry->next;
		free(old_entry);
	}
	free(self);
	self = NULL;
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
void *get(HashTable *table, char *key);
int in_hash_table(HashTable *self, char *key);
HashEntry *get_entry_at_key(HashTable *self, char *key);
void add_key(HashTable *table, char *key, void *value);
void change_hash_table_size(HashTable *table, int new_max_size);
void remove_hash_entry(HashTable *self, char *key);
void free_entry(HashEntry *e);
char **hash_table_keys(HashTable *self);
void print_hash_table(HashTable *self);
void print_hash_entry(HashEntry *e, int index, char type);

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

	new_table->current_size = 0;
	new_table->max_size = size;
	new_table->type = type;
	//asign the function pointers
	new_table->add = add_key;
	new_table->get = get;
	new_table->print = print_hash_table;
	new_table->remove = remove_hash_entry;
	new_table->in = in_hash_table;
	new_table->keys = hash_table_keys;
	return new_table;
}

/*djb2 --> http://www.cse.yorku.ca/~oz/hash.html tested to be a very decent and simple hash function*/
unsigned hash(char *key, int hash_table_size)
{
	unsigned long hash = 5381;
	int c;

	while (c = *key++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return ((unsigned)hash % hash_table_size);
}

/*Get a value corresponding to a key from the hashtable*/
void *get(HashTable *self, char *key) {
	HashEntry *result = get_entry_at_key(self, key);
	if (result == NULL) {
		printf("Keyerror. Key '%s' not in hashtable\n", key);
		exit(1);
	}
	return result->value;
}

/*Check if a key is in a hashtable or not*/
int in_hash_table(HashTable *self, char *key) {
	if (get_entry_at_key(self, key) == NULL) {
		return False;
	}
	return True;
}

HashEntry *get_entry_at_key(HashTable *self, char *key) {
	HashEntry *entry_pointer;

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
void add_key(HashTable *self, char *key, void *value) {
	HashEntry *entry_pointer;
	unsigned hash_value;

	//no key found with name of key --> add new value
	if ((entry_pointer = get_entry_at_key(self, key)) == NULL) {
		self->current_size++;

		entry_pointer = malloc(sizeof(*entry_pointer));
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
	// replace old value
	else {
		//free the memory for a new value
		free(entry_pointer->value);
	}
	//place the value in the table and check for storage trouble
	if ((entry_pointer->value = _strdup(value)) == NULL) {
		printf("Not enough memory to place new entry in table\n");
		exit(1);

	}

	// when the dictionary becomes to big collision becomes more likely, so increase the size
	if (self->current_size > (int)(self->max_size * MAX_HASH_TABLE_RACTION_FILL)) {
		change_hash_table_size(self, self->max_size * 2);
	}
}

void change_hash_table_size(HashTable *hash_table, int new_max_size) {
	int orig_size = hash_table->max_size;
	HashEntry **orig_table = hash_table->table;
	hash_table->max_size = new_max_size;
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
void remove_hash_entry(HashTable *self, char *key) {
	HashEntry *ep1, *ep2;

	/* create 2 pointers, 1 to the current and one to the previous element */
	for (ep1 = ep2 = self->table[hash(key, self->max_size)]; ep1 != NULL; ep2 = ep1, ep1 = ep1->next) {
		// when an actual match is found 
		if (strcmp(key, ep1->key) == 0) {
			self->current_size--;
			// when making a new hash entry
			if (ep1 == ep2) {
				self->table[hash(key, self->max_size)] = ep1->next;
			}
			// when collision occurs
			else {
				ep2->next = ep1->next;
			}
			// when the dictionary becomes to big collision becomes more likely, so increase the size
			if (self->current_size < (int)(self->max_size * MIN_HASH_TABLE_FRACTION_FILL)) {
				change_hash_table_size(self, self->max_size / 2);
			}

			/*  Free memory  */
			free_entry(ep1);
			return;
		}
	}
	/* No key found */
	printf("Keyerror. Key '%s' not in hashtable\n", key);
	exit(1);
}

void free_entry(HashEntry *e) {
	free(e->key);
	free(e);
}

char **hash_table_keys(HashTable *self) {
	char **keys;
	keys = malloc(sizeof(*keys) * (self->current_size + 1));
	int values_index = 0;
	for (int index = 0; index < self->max_size; index++) {
		HashEntry *entry = self->table[index];
		while (entry != NULL) {
			keys[values_index] = entry->value;
			values_index++;
			entry = entry->next;
		}
	}
	keys[values_index] = NULL;
	return keys;
}

/*print a simple version of the hashtable showing keys and values*/
void print_hash_table(HashTable *self) {
	printf("{");
	int printed_first = False;
	for (int i = 0; i < self->max_size; i++) {
		HashEntry *entry_pointer = self->table[i];
		if (entry_pointer != NULL) {
			do {
				if (printed_first != False) {
					printf(", ");
				}
				else {
					printed_first = True;
				}
				printf("%s: ", entry_pointer->key);
				print_type(self->type, entry_pointer->value);
			} while ((entry_pointer = entry_pointer->next) != NULL);
		}
	}
	printf("}\n");
}

/*Function for printing the hashtable with additional information displaying the table and collisions*/
void print_full_hash_table(HashTable *self) {
	int i;

	printf("Hastable of size %d:{\n", self->max_size);
	for (i = 0; i < self->max_size; i++) {
		HashEntry *entry_pointer = self->table[i];
		if (entry_pointer != NULL) {
			print_hash_entry(entry_pointer, i, self->type);
		}
	}
	printf("}\n");
}

/*Recursive function for printing entries of the hashtable*/
void print_hash_entry(HashEntry *e, int index, char type) {
	//if the index is bigger it is a genuine index. Else it is an indicator that is a repeat.
	if (index >= 0) {
		printf("\tEntry %d = %s: ", index, e->key);
	}
	else {
		printf(", %s: ", e->key);
	}
	print_type(type, e->value);
	if (e->next != NULL) {
		print_hash_entry(e->next, -1, type);
	}
	else {
		printf("\n");
	}
}

// SET FUNCTIONS

char **set_values(Set *self);
void add_set_entry(Set *self, char *value);
int value_in_set(Set *self, char *value);
void remove_set_entry(Set *self, char *value);
void change_set_size(Set *set, int new_max_size);
void print_set(Set *self);
void print_full_set(Set *self);
void print_set_entry(SetEntry *e, int index);

Set *new_set(int size) {
	int i;
	Set *new_set;

	//make sure the size is bigger then 0
	if (size < 1) {
		printf("Cannot instantiate set smaller then 1.");
		exit(1);
	}

	//allocate the set
	new_set = malloc(sizeof(*new_set));
	if (new_set == NULL) {
		printf("Cannot allocate memory for the new Set\n");
		exit(1);
	}
	// Allocate pointers to the head nodes.
	new_set->table = (SetEntry **)malloc(sizeof(SetEntry *) * size);
	if (new_set->table == NULL) {
		printf("Cannot allocate memory for the new Set table\n");
		exit(1);
	}
	for (i = 0; i < size; i++) {
		new_set->table[i] = NULL;
	}

	new_set->max_size = size;
	new_set->current_size = 0;
	new_set->add = add_set_entry;
	new_set->in = value_in_set;
	new_set->print = print_set;
	new_set->remove = remove_set_entry;
	new_set->values = set_values;
	return new_set;
}

char **set_values(Set *self) {
	char **values;
	values = malloc(sizeof(*values) * (self->current_size + 1));
	int values_index = 0;
	for (int index = 0; index < self->max_size; index++) {
		SetEntry *entry = self->table[index];
		while (entry != NULL) {
			values[values_index] = entry->value;
			values_index++;
			entry = entry->next;
		}
	}
	values[values_index] = NULL;
	return values;
}

void add_set_entry(Set *self, char *value) {
	SetEntry *new_entry;
	unsigned hash_value;

	// if value not in set
	if (value_in_set(self, value) == False) {

		self->current_size++;

		new_entry = (SetEntry*)malloc(sizeof(SetEntry));
		//check if there is a problem allocating memory 
		if (new_entry == NULL) {
			printf("Cannot allocate memory for the new Entry\n");
			exit(1);
		}
		new_entry->value = _strdup(value);
		if(new_entry->value == NULL){
			printf("Cannot allocate memory for the new Entry\n");
			exit(1);
		}

		//assign the current entry to the next and the new one to the current
		hash_value = hash(value, self->max_size);
		new_entry->next = self->table[hash_value];
		self->table[hash_value] = new_entry;
	}
	// just exit
	else {
		return;
	}

	// when the dictionary becomes to big collision becomes more likely, so increase the size
	if (self->current_size > (int)(self->max_size * MAX_HASH_TABLE_RACTION_FILL)) {
		change_set_size(self, self->max_size * 2);
	}

}

void remove_set_entry(Set *self, char *value) {
	SetEntry *ep1, *ep2;

	/* create 2 pointers, 1 to the current and one to the previous element */
	for (ep1 = ep2 = self->table[hash(value, self->max_size)]; ep1 != NULL; ep2 = ep1, ep1 = ep1->next) {
		// when an actual match is found 
		if (strcmp(value, ep1->value) == 0) {
			self->current_size--;

			// when valur matches the first value of the row
			if (ep1 == ep2) {
				self->table[hash(value, self->max_size)] = ep1->next;
			}
			// other cases
			else {
				ep2->next = ep1->next;
			}
			// when the dictionary becomes to big collision becomes more likely, so increase the size
			if (self->current_size < (int)(self->max_size * MIN_HASH_TABLE_FRACTION_FILL)) {
				change_set_size(self, self->max_size / 2);
			}

			/*  Free memory  */
			free(ep1);
			return;
		}
	}
	/* No key found */
	printf("KeyError. Key '%s' not in set\n", value);
	exit(1);
}

/*Check if a value is in a set 0 if not 1 if in.*/
int value_in_set(Set *self, char *value) {
	SetEntry *entry_pointer;
	//Look for an entry as long as there are more entries, in case of collisions
	for (entry_pointer = self->table[hash(value, self->max_size)]; entry_pointer != NULL; entry_pointer = entry_pointer->next) {
		//check if the name of the pointer is that of the key
		if (strcmp(value, entry_pointer->value) == 0) {
			return True;
		}
	}
	return False;
}

void change_set_size(Set *set, int new_max_size) {
	int orig_size = set->max_size;
	SetEntry **orig_table = set->table;
	set->max_size = new_max_size;
	set->current_size = 0;

	set->table = malloc(sizeof(SetEntry *) * set->max_size);
	if (set->table == NULL) {
		printf("Cannot allocate memory for new hash table\n");
		exit(1);
	}

	for (int index = 0; index < set->max_size; index++) {
		set->table[index] = NULL;
	}

	// copy values
	for (int index = 0; index < orig_size; index++) {
		SetEntry *entry = orig_table[index];
		while (entry != NULL) {
			set->add(set, entry->value);
			entry = entry->next;
		}
	}
	free(orig_table);
}

/*print a simple representation of a set with all the values in any order*/
void print_set(Set *self) {
	printf("{");
	int printed_first = False;
	for (int i = 0; i < self->max_size; i++) {
		SetEntry *entry_pointer = self->table[i];
		if (entry_pointer != NULL) {
			do {
				if (printed_first != False) {
					printf(", ");
				}
				else {
					printed_first = True;
				}
				printf("%s", entry_pointer->value);
			} while ((entry_pointer = entry_pointer->next) != NULL);
		}
	}
	printf("}\n");
}

void print_full_set(Set *self) {
	int i;

	printf("Set of size %d:{\n", self->max_size);
	for (i = 0; i < self->max_size; i++) {
		SetEntry *entry_pointer = self->table[i];
		if (entry_pointer != NULL) {
			print_set_entry(entry_pointer, i);
		}
	}
	printf("}\n");
}

/*Recursive function for printing entries of the hashtable*/
void print_set_entry(SetEntry *e, int index) {
	//if the index is bigger it is a genuine index. Else it is an indicator that is a repeat.
	if (index >= 0) {
		printf("\tEntry %d = %s", index, e->value);
	}
	else {
		printf(", %s", e->value);
	}
	
	if (e->next != NULL) {
		print_set_entry(e->next, -1);
	}
	else {
		printf("\n");
	}
}

// TESTING FUNCTIONS

int test_linked_list() {
	printf("Start LinkedList tests:\n");
	LinkedList *test = new_linked_list('s');
	test->print(test);
	test->append(test, "test1");
	test->print(test);
	test->to_array(test, 0, test->size);
	test->append(test, "test2");
	test->append(test, "test3");
	test->append(test, "test4");
	test->print(test);

	LinkedList *test2 = new_linked_list('d');
	int i = 1;
	int j = 5;
	test2->append(test2, &i);
	test2->append(test2, &j);
	test2->print(test2);
	test->to_array(test, 0, test->size);

	test2->delete(test2);

	printf("LinkedList tests finished succesfully\n\n");
	return 0;
}

int test_hash_table() {
	printf("Start HashTable tests:\n");
	HashTable *test_table = new_hash_table(4, 's');
	test_table->print(test_table);
	test_table->add(test_table, "key1", "value1");
	test_table->add(test_table, "key1", "value2");
	test_table->add(test_table, "key_number_2", "value2");
	test_table->add(test_table, "key3", "value24");

	test_table->print(test_table);
	print_full_hash_table(test_table);
	test_table->add(test_table, "key4", "value23");

	test_table->print(test_table);
	print_full_hash_table(test_table);

	printf("%s\n", (char *)test_table->get(test_table, "key1"));
	test_table->remove(test_table, "key1");
	test_table->remove(test_table, "key3");
	test_table->remove(test_table, "key4");
	print_full_hash_table(test_table);

	test_table->remove(test_table, "key_number_2");
	print_full_hash_table(test_table);

	printf("Hashtable tests finished succesfully\n\n");
	return 0;
}

int test_set() {
	printf("Start Set tests:\n");
	char** values;
	char *line;
	Set *test_set = new_set(2);
	test_set->print(test_set);
	print_full_set(test_set);

	test_set->add(test_set, "test1");
	test_set->print(test_set);
	print_full_set(test_set);

	test_set->add(test_set, "test1");
	test_set->add(test_set, "test2");
	test_set->add(test_set, "test_2");
	test_set->add(test_set, "test_3");
	test_set->add(test_set, "test_4");

	test_set->print(test_set);
	print_full_set(test_set);

	values = test_set->values(test_set);
	while ((line = *values++) != NULL) {
		printf("%s, ", line);
	}
	printf("\n");

	test_set->remove(test_set, "test1");
	test_set->remove(test_set, "test2");
	test_set->remove(test_set, "test_4");
	test_set->remove(test_set, "test_3");
	test_set->print(test_set);
	print_full_set(test_set);

	printf("Set tests finished succesfully\n\n");
	return 0;
}

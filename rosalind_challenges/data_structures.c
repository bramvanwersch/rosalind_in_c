#include <stdio.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

#define ERROR_BUFFER_SIZE 100

// percentage a table is filled before the size is increased
#define MAX_HASH_TABLE_FRACTION_FILL 0.8
// this value should be chosen in sutch a way that shrinking does not cause
// subsequent growing of the table
#define MIN_HASH_TABLE_FRACTION_FILL 0.3 

// start size of a hashTable when innitialising
#define INNITIAL_TABLE_SIZE 2

/*
---------------------
GENERAL USE FUNCTIONS
---------------------
Functions used by most datastructures in this file.
*/

void print_type(char type, void *value);
unsigned hash(char *key, int size);
void raise_memory_error(char *message);
void raise_value_error(char *message);


/*
Print a value given a type of that value

type (char): type used for printing. Available options are c=char, s=string, 
	d=digit, f=double and p=pointer
value (void pointer): value of any type that should match type. Otherwise 
	undifined behaviour.
*/
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


/*
Hash function from --> http://www.cse.yorku.ca/~oz/hash.html tested to be a 
very decent and simple hash function

key (char pointer): character array to be hashed. Has to be null terminated.
container_size (int): the size of the container the hash needs to map to.
*/
unsigned hash(char *key, int container_size)
{
	unsigned long hash = 5381;
	int c;

	while (c = *key++) {
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return ((unsigned)hash % container_size);
}


/*
Signify the user there is not enough memory and exit

message (char pointer): null terminated character array with an additional 
	message.
*/
void raise_memory_error(char *message) {
	printf("MemoryError: Cannot allocate enought memory. %s\n", message);
	exit(1);
}


/*
Signify the user there an input is invalid and exit

message (char pointer): null terminated character array with an additional
	message.
*/
void raise_value_error(char *message) {
	printf("ValueError: Invalid value. %s\n", message);
	exit(2);
}


/*
---------------------
LINKED LIST FUNCTIONS
---------------------
Functions for creation and management of a LinkedList data structure. 
*/

// public
LinkedEntry *new_linked_entry();

// private
void append_entry(LinkedList *self, void *value, int sizeof_value);
void **to_array(LinkedList *self, int from, int until);
void copy_linked_list_pointers(LinkedList *self, void **out_array, int from,
	int until);
void delete_linked_list(LinkedList *self);
void delete_linked_entry(LinkedEntry *entry);
void print_linked_list(LinkedList *self);


/*
Create a new LinkedList

type (char): type used for certain type dependant operations. Available 
	options are c=char, s=string, d=digit, f=double and p=pointer

Returns (LinkedList pointer): pointer to the newly innitialised LinkedList 
*/
LinkedList *new_linked_list(char type) {
	LinkedList *new_list;
	new_list = malloc(sizeof(*new_list));
	if (new_list == NULL) {
		raise_memory_error("LinkedList creation failed.");
	}
	LinkedEntry *root_entry = new_linked_entry();

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


/*
Create a LinkedEntry. These are entries of a linked list.

Returns (LinkedEntry pointer): pointer to the newly innitialised entry
*/
LinkedEntry *new_linked_entry() {
	LinkedEntry *new_entry;
	new_entry = (LinkedEntry *)malloc(sizeof(*new_entry));
	if (new_entry == NULL) {
		raise_memory_error("LinkedList creation failed.");
	}
	return new_entry;
}


/*
Add an a new value to the end of the LinkedList. The value is coppied so it is
safe to free or dereference it after.

self (LinkedList pointer): the LinkedList to append the value to.
value (void pointer): value of any type that is appended. All values of a 
	LinkedList should have the same type, otherwise undefined behaviour can
	occur.
*/
void append_entry(LinkedList *self, void *value, int sizeof_value) {
	LinkedEntry* new_entry = new_linked_entry();

	//place value at the current end
	self->end->value = malloc(sizeof_value);
	memcpy(self->end->value, value, sizeof_value);
	if (self->end->value == NULL) {
		raise_memory_error("Failed to copy value.");
	}
	
	// add a new empty entry at the end
	self->end->next = new_entry;
	new_entry->next = NULL;
	new_entry->value = NULL;
	self->end = new_entry;
	
	self->size++;
}


/*
Convert a LinkedList into a list of void pointers

self (LinkedList pointer): the LinkedList to extract the array from
from (int): positive integer
until (int): positive integer

Returns (void pointer pointer): a slice of the input LinkedList
*/
void **to_array(LinkedList *self, int from, int until) {
	if (until < 0 || from < 0 || until <= from) {
		raise_value_error("From has to be bigger or equal to until.");
	}

	// make sure that not more then max of the list is selected
	until = MIN(self->size, until);

	void **out_array;
	out_array = malloc(sizeof(*out_array) * ((until - from) + 1));
	if (out_array == NULL) {
		raise_memory_error("Cannot allocate memory for LinkedList.");
	}

	copy_linked_list_pointers(self, out_array, from, until);
	return out_array;
}


/*
Copy pointers from the LinkedList into the out_array

self (LinkedList pointer): the LinkedList to retrieve the pointers from.
out_array (void pointer pointer): the list to put the LinkedList values into.
from (int): valid positive integer
until (int): valid positive integer
*/
void copy_linked_list_pointers(LinkedList *self, void **out_array, int from,
	int until) {
	LinkedEntry *entry = self->root;
	int index = 0;
	int out_index = 0;
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
}


/*
Delete a LinkedList instance by freeing all entrys and the LinkedList itself

NOTE: values are not freed because they are not guaranteed to be malloced and 
	the user might still need them.

self (LinkedList pointer): the LinkedList to delete.
*/
void delete_linked_list(LinkedList *self) {
	LinkedEntry *entry = self->root;
	LinkedEntry *old_entry;
	while (entry->next != NULL) {
		old_entry = entry;
		entry = entry->next;
		delete_linked_entry(old_entry);
	}
	free(self);
}


/*
Safely delete a LinkedEntry

entry (LinkedEntry pointer): pointer to the entry to delete.
*/
void delete_linked_entry(LinkedEntry *entry) {
	free(entry->value);
	free(entry);
}

/*
Fancy print of a LinkedList in a python syntax style

self (LinkedList pointer): the LinkedList to print.
*/
void print_linked_list(LinkedList *self) {
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


/* 
-------------------
HASHTABLE FUNCTIONS 
-------------------

Functions for instantiation maintenance and manipulation of a HashTable
*/

// Available from header (public)
HashTable *new_hash_table(char type);

// private
HashEntry **create_hash_table_table(int size);

HashEntry *new_hash_entry();
void delete_hash_entry(HashEntry *e);
void raise_key_error(char *key);
void *get_value(HashTable *table, char *key);
int in_hash_table(HashTable *self, char *key);
HashEntry *hash_entry_from_key(HashTable *self, char *key);
void add_key_value_pair(HashTable *table, char *key, void *value, int sizeof_value);
void change_hash_table_size(HashTable *table, int new_max_size);
void remove_hash_entry(HashTable *self, char *key);
char **hash_table_keys(HashTable *self);
void print_hash_table(HashTable *self);
void print_full_hash_table(HashTable *self);
void print_hash_entry(HashEntry *e, int index, char type);


/*
Create a new hash table

type (char): type used for certain type dependant operations. Available
	options are c=char, s=string, d=digit, f=double and p=pointer

Returns (HashTable pointer): a pointer to a HashTable
*/
HashTable *new_hash_table(char type) {

	HashTable *new_table;
	new_table = malloc(sizeof(*new_table));
	if (new_table == NULL) {
		raise_memory_error("Failed to allocate memory for the new HashTable\
						   structure.");
	}

	new_table->table = create_hash_table_table(INNITIAL_TABLE_SIZE);
	
	//values
	new_table->current_size = 0;
	new_table->max_size = INNITIAL_TABLE_SIZE;
	new_table->type = type;

	// functions
	new_table->add = add_key_value_pair;
	new_table->get = get_value;
	new_table->print = print_hash_table;
	new_table->remove = remove_hash_entry;
	new_table->in = in_hash_table;
	new_table->keys = hash_table_keys;
	return new_table;
}


/*
Create a array of NULLED HashEntry pointers for a given size

size(int): the size of the array

Returns(HashEntry pointer pointer): an array of pointers to NULLED hash entries
*/
HashEntry **create_hash_table_table(int size) {
	HashEntry **new_table;
	if (size <= 0) {
		raise_value_error("Failed to create HashTable of the given size. Size \
			has to be bigger than 0.");
	}
	new_table = malloc(sizeof(*new_table) * size);
	if (new_table == NULL) {
		raise_memory_error("Failed to allocate memory for table of HashTable.");
	}

	for (int index = 0; index < size; index++) {
		new_table[index] = NULL;
	}
	return new_table;
}


/*
Create a HashEntry and raise memory error when appropriate

Returns (HashEntry pointer): a pointer to a HashEntry.
*/
HashEntry *new_hash_entry() {
	HashEntry *entry_pointer;
	entry_pointer = malloc(sizeof(*entry_pointer));
	if (entry_pointer == NULL) {
		raise_memory_error("Failed to locate memory for HashEntry.");
	}
	entry_pointer->next = NULL;
	entry_pointer->key = NULL;
	entry_pointer->value = NULL;
	return entry_pointer;
}


/*
Safely delete a HashEntry and all contained information

entry (HashEntry pointer): the entry to delete.
*/
void delete_hash_entry(HashEntry *entry) {
	free(entry->key);
	free(entry->value);
	free(entry);
}


/*
Signify the user that there is no key with the given key name

key (char pointer): null terminated character array that is the name of a key
	not in the HashTable
*/
void raise_key_error(char *key) {
	printf("Keyerror. Key '%s' not in HashTable\n", key);
	exit(3);
}


/*
Get a value corresponding to a key from the hashtable

self (HashTable pointer): the HashTable retrieve the value from.
key (char pointer): key that corresponds to value

Returns (void pointer): value saved at the given key. 
*/
void *get_value(HashTable *self, char *key) {
	HashEntry *result = hash_entry_from_key(self, key);
	if (result == NULL) {
		raise_key_error(key);
	}
	return result->value;
}


/*
Check if a key is in a hashtable or not. If you do not want key errors use this
method to check for keys

self (HashTable pointer): the HashTable to test for the key
key (char pointer): key that corresponds to value

Returns (int): 0 if key is not present 1 if it is present. (can use True or 
	False to test as well).
*/
int in_hash_table(HashTable *self, char *key) {
	if (hash_entry_from_key(self, key) == NULL) {
		return False;
	}
	return True;
}


/*
Retrieve a HashEntry associated with a key return NULL if no sutch ket exists

self (HashTable pointer): the HashTable to retrieve the HashEntry from.
key (char pointer): key that corresponds to value

Returns (HashEntry pointer || NULL): a HashEntry pointer if the ket is present
	in self otherwise NULL.
*/
HashEntry *hash_entry_from_key(HashTable *self, char *key) {
	HashEntry *entry_pointer;

	//Look for an entry as long as there are more entries at a hash of a key
	for (entry_pointer = self->table[hash(key, self->max_size)];
		entry_pointer != NULL; entry_pointer = entry_pointer->next) {
		//check if the name of the pointer is that of the key
		if (strcmp(key, entry_pointer->key) == 0) {
			return entry_pointer;
		}
	}
	//no key found
	return NULL;
}


/*
Add a key to the hashtable or replace the old value, with a copy of the 
provided value. Meaning it is fine to delete the value after placing it in the
HashTable.

self (HashTable pointer): the HashTable to place the value into.
key (char pointer): key to place the value at.
value (void pointer): value to be placed at the key
sizeof_value (int): the size of the value in order to copy the value.
*/
void add_key_value_pair(HashTable *self, char *key, void *value,
	int sizeof_value) {
	HashEntry *new_entry;
	new_entry = hash_entry_from_key(self, key);

	//no key found with name of key, add a new value
	if (new_entry == NULL) {
		self->current_size++;

		new_entry = new_hash_entry();
		new_entry->key = _strdup(key);
		if (new_entry->key == NULL) {
			raise_memory_error("Failed to duplicate key.");
		}

		//assign the current entry to the next and the new one to the current
		unsigned hash_value = hash(key, self->max_size);
		new_entry->next = self->table[hash_value];
		self->table[hash_value] = new_entry;
	}
	else {
		//free the memory for a new value
		free(new_entry->value);
	}

	// copy the value into a HashEntry in order to be able to safely free it
	new_entry->value = malloc(sizeof_value);
	memcpy(new_entry->value, value, sizeof_value);
	if (new_entry->value == NULL) {
		raise_memory_error("Failed to copy value.");
	}

	// when the dictionary becomes to full, increase size
	if (self->current_size > 
		(int)(self->max_size * MAX_HASH_TABLE_FRACTION_FILL)) {
		change_hash_table_size(self, self->max_size * 2);
	}
}


/*
Change the size of a HashTable's table

hash_table (HashTable pointer): a pointer to the HashTable that has to be 
	increased.
new_max_size (int): new size for the table. Has to be bigger than 0.
*/
void change_hash_table_size(HashTable *hash_table, int new_max_size) {
	int orig_size = hash_table->max_size;
	HashEntry **orig_table = hash_table->table;
	hash_table->max_size = new_max_size;
	hash_table->current_size = 0;

	hash_table->table = create_hash_table_table(hash_table->max_size);

	// repoint the entries into the new table
	for (int index = 0; index < orig_size; index++) {
		HashEntry *entry = orig_table[index];
		HashEntry *orig_entry = NULL;
		while (entry != NULL) {
			unsigned hash_val = hash(entry->key, hash_table->max_size);
			orig_entry = entry;
			entry = entry->next;

			orig_entry->next = hash_table->table[hash_val];
			hash_table->table[hash_val] = orig_entry;
			hash_table->current_size++;
		}
	}
	free(orig_table);
}


/*
Delete a key and value pair from the HashTable.

self (HashTable pointer): the HashTable to remove the value from.
key (char pointer): key to delete the value from.
*/
void remove_hash_entry(HashTable *self, char *key) {
	HashEntry *current_entry, *previous_entry;

	/* create 2 pointers, 1 to the current and one to the previous element */
	for (current_entry = previous_entry = self->table[hash(key, self->max_size)];
		current_entry != NULL; current_entry = current_entry->next) {
		// when an actual match is found 
		if (strcmp(key, current_entry->key) == 0) {
			self->current_size--;
			if (current_entry == previous_entry) {
				self->table[hash(key, self->max_size)] = current_entry->next;
			}
			else {
				previous_entry->next = current_entry->next;
			}
			//decrease the dict if the table is too empty
			if (self->current_size <
				(int)(self->max_size * MIN_HASH_TABLE_FRACTION_FILL)) {
				change_hash_table_size(self, self->max_size / 2);
			}

			delete_hash_entry(current_entry);
			return;
		}
		else {
			previous_entry = current_entry;
		}
	}
	raise_key_error(key);
}


/*
Get an array of all keys in a HashTable. The order is not guaranteed

self (HashTable pointer): pointer to the hashtable to retrieve the keys from

Returns (char pointer pointer): return a null terminated array of strings.
*/
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


/*
Print a A pythonlike representation of a HashTable

self (HashTable pointer): pointer a HashTable to print.
NOTE: printing may fail if the type given to the HashTable and the actual type
	of the values do not match.
*/
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


/*
Function for printing a HashTable with all relevant information (size,
collision)

self (HashTable pointer): pointer a HashTable to print.
*/
void print_full_hash_table(HashTable *hash_table) {
	int i;

	printf("Hastable of size %d:{\n", hash_table->max_size);
	for (i = 0; i < hash_table->max_size; i++) {
		HashEntry *entry_pointer = hash_table->table[i];
		if (entry_pointer != NULL) {
			print_hash_entry(entry_pointer, i, hash_table->type);
		}
	}
	printf("}\n");
}


/*
Recursively print all entries at a given table row.

entry (HashEntry pointer): pointer to a HashEntry acting as the row.
row_index (int): genuine index if this is the first call of the recursive stack
	-1 otherwise.
type (char): a character that is the type of the HashTable the entry originates
	from.
*/
void print_hash_entry(HashEntry *entry, int row_index, char type) {
	if (row_index >= 0) {
		printf("\tEntry %d = %s: ", row_index, entry->key);
	}
	else {
		printf(", %s: ", entry->key);
	}
	print_type(type, entry->value);
	if (entry->next != NULL) {
		print_hash_entry(entry->next, -1, type);
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
	if (self->current_size > (int)(self->max_size * MAX_HASH_TABLE_FRACTION_FILL)) {
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
	int sizeof_str = sizeof(char *);
	LinkedList *test = new_linked_list('s');
	test->print(test);
	test->append(test, "test1", sizeof_str);
	test->print(test);
	test->to_array(test, 0, test->size);
	test->append(test, "test2", sizeof_str);
	test->append(test, "test3", sizeof_str);
	test->append(test, "test4", sizeof_str);
	test->print(test);

	LinkedList *test2 = new_linked_list('d');
	int i = 1;
	int j = 5;
	test2->append(test2, &i, sizeof(int));
	test2->append(test2, &j, sizeof(int));
	test2->print(test2);
	test->to_array(test, 0, test->size);

	test2->delete(test2);

	printf("LinkedList tests finished succesfully\n\n");
	return 0;
}

int test_hash_table() {
	printf("Start HashTable tests:\n");
	int sizeof_str = sizeof(char *);
	HashTable *test_table = new_hash_table('s');
	test_table->print(test_table);
	print_full_hash_table(test_table);
	test_table->add(test_table, "key1", "value1", sizeof_str);
	test_table->add(test_table, "key1", "value2", sizeof_str);
	test_table->add(test_table, "key_number_2", "value2", sizeof_str);
	test_table->add(test_table, "key3", "value24", sizeof_str);

	test_table->print(test_table);
	print_full_hash_table(test_table);
	test_table->add(test_table, "key4", "value23", sizeof_str);

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

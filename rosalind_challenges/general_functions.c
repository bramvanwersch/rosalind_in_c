#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "general_functions.h"

#define ERROR_BUFFER_SIZE 100

FILE* open_file(char *file_name, const char *mode) {
	// open a file with error checking
	errno_t error;
	FILE *file_pointer;
	if ((error = fopen_s(&file_pointer, file_name, mode)) != 0) {
		// When an error occurs print the error to standard error out
		char buf[ERROR_BUFFER_SIZE];
		strerror_s(buf, sizeof buf, error);
		fprintf_s(stderr, "cannot open file '%s': %s\n", file_name, buf);
		exit(1);
	}
	return file_pointer;
}

char* read_file(FILE *file_pointer) {
	// read a file into a null terminated string
	int long nr_bytes;
	char *buffer = 0;

	fseek(file_pointer, 0, SEEK_END);
	nr_bytes = ftell(file_pointer);
	fseek(file_pointer, 0, SEEK_SET);

	buffer = malloc(nr_bytes);
	if (buffer) {
		fread(buffer, 1, nr_bytes, file_pointer);
	}
	// make sure to null terminate the string
	buffer[nr_bytes] = '\0';
	return buffer;
}

LinkedList* read_lines(FILE *file_pointer) {
	// read all lines of a file into an array of null terminated strings
	int long nr_bytes;
	char *buffer = 0;
	int buffer_index = 0;

	int long line_byte_size;
	char ch;
	char *line;
	LinkedList* root = allocate_linked_entry();
	LinkedList* new_line = root;

	// allocate sufficient memory for any possible line --> might be overkill or realy bad, not sure
	fseek(file_pointer, 0, SEEK_END);
	nr_bytes = ftell(file_pointer);
	fseek(file_pointer, 0, SEEK_SET);
	if ((buffer = malloc(nr_bytes)) == NULL) {
		free(buffer);
		printf("malloc ran out of memory when allocating memory for the line buffer.");
		return NULL;
	}

	while ((ch = fgetc(file_pointer))) {
		if (ch == '\n' || ch == EOF) {
			buffer[buffer_index] = '\0';
			line_byte_size = sizeof(char) * (buffer_index + 1); // to include null pointer
			if ((line = malloc(line_byte_size)) == NULL) {
				free(buffer);
				free(line);
				printf("malloc ran out of memory when allocating memory for the next line.");
				return NULL;
			}

			strcpy_s(line, buffer_index + 1, buffer);
			new_line = add_linked_entry(new_line, line);
			buffer_index = 0;
			if (ch == EOF) {
				break;
			}
		}
		else {
			buffer[buffer_index] = ch;
			buffer_index++;
		}
	}
	free(buffer);
	return root;
}

LinkedList* allocate_linked_entry(void) {
	return (LinkedList *)malloc(sizeof(LinkedList));
}

LinkedList* add_linked_entry(LinkedList *entry, char* line) {
	LinkedList* new_entry;

	new_entry = allocate_linked_entry();
	entry->value = line;
	new_entry->next = NULL;
	new_entry->value = NULL;
	entry->next = new_entry;
	return new_entry;
}

unsigned hash(char *key, int size);
char *get(HashTable *table, char *key);
Entry *in(HashTable *self, char *key);
Entry *add(HashTable *table, char *key, char *value);
void increase_table_size(HashTable *table);
int delete(HashTable *self, char *key);
void free_entry(Entry *e);
void print(HashTable *self);
void printEntry(Entry *e, int index);

/*Create a new hash table*/
HashTable *new_hash_table(int size) {
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
	if ((new_table->table = malloc(sizeof(Entry *) * size)) == NULL) {
		printf("Cannot allocate memory for the new HashTable table\n");
		exit(1);
	}
	for (i = 0; i < size; i++) {
		new_table->table[i] = NULL;
	}

	new_table->max_size = size;
	//asign the function pointers
	new_table->add = add;
	new_table->get = get;
	new_table->print = print;
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
	Entry *result = in(self, key);
	if (result == NULL) {
		printf("Keyerror. Key '%s' not in hashtable", key);
	}
	return result->value;
}

Entry *in(HashTable *self, char *key) {
	struct Entry *entry_pointer;

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
Entry *add(HashTable *self, char *key, char *value) {
	Entry *entry_pointer;
	unsigned hash_value;
	self->current_size++;
	
	// when the dictionary becomes to big collision becomes more likely, so increase the size
	if (self->current_size > (int) (self->max_size * 0.8)) {
		increase_table_size(self);

	}

	//no key found with name of key
	if ((entry_pointer = in(self, key)) == NULL) {
		entry_pointer = (Entry *)malloc(sizeof(*entry_pointer));
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
	Entry **orig_table = hash_table->table;
	hash_table->max_size *= 2;
	hash_table->current_size = 0;

	// reallocate the table
	if ((hash_table->table = malloc(sizeof(Entry *) * hash_table->max_size)) == NULL) {
		printf("Cannot allocate memory for new hash table\n");
		exit(1);
	}

	for (int index = 0; index < hash_table->max_size; index++) {
		hash_table->table[index] = NULL;
	}

	// copy keys
	for (int index = 0; index < orig_size; index++) {
		Entry *entry = orig_table[index];
		while (entry != NULL) {
			hash_table->add(hash_table, entry->key, entry->value);
			entry = entry->next;
		}
	}
	free(orig_table);
}

/* Delete a key and value from the hashtable. Return 0 on succes and 1 on failure */
int delete(HashTable *self, char *key) {
	Entry *ep1, *ep2;
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

void free_entry(Entry *e) {
	free(e->value);
	free(e->key);
	free(e);
}

/*Function for printing the hashtable*/
void print(HashTable *self) {
	int i;

	printf("Hastable of size %d:{\n", self->max_size);
	for (i = 0; i < self->max_size; i++) {
		Entry *entry_pointer = self->table[i];
		if (entry_pointer != NULL) {
			printEntry(entry_pointer, i);
		}
	}
	printf("}\n");
}

/*Recursive function for printing entries of the hashtable*/
void printEntry(Entry *e, int index) {
	//if the index is bigger it is a genuine index. Else it is an indicator that is a repeat.
	if (index >= 0) {
		printf("\tEntry %d, %s: %s", index, e->key, e->value);
	}
	else {
		printf(", %s: %s", e->key, e->value);
	}
	if (e->next != NULL) {
		printEntry(e->next, -1);
	}
	else {
		printf("\n");
	}
}
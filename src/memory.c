#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

#define MIN_MEMORY_SIZE 10

void memory_init (Memory *memory) {
	memory->memory = (int*) malloc (sizeof (int) * MIN_MEMORY_SIZE);
	memory->goodbits = (int*) malloc (sizeof (int) * MIN_MEMORY_SIZE);
	for (int i = 0; i < MIN_MEMORY_SIZE; i++) memory->goodbits[i] = -1;
	memory->size = MIN_MEMORY_SIZE;
	memory->max_index = 0;
}

void memory_destroy (Memory *memory) {
	free (memory->memory);
	memory->memory = NULL;
	free (memory->goodbits);
	memory->goodbits = NULL;
	memory->max_index = memory->size = 0;
}

int  memory_new (Memory *memory, int size) {
	if (size <= 0) {
		exit (1);
	}

	int where = memory->max_index;
	int need = where + size;
	int newsize = memory->size;
	while (newsize <= need) newsize *= 2;

	if (newsize != memory->size) {
		memory->memory = (int*) realloc (memory->memory, sizeof (int) * newsize);
		memory->goodbits = (int*) realloc (memory->goodbits, sizeof (int) * newsize);
		for (int i = memory->size; i < newsize; i++) memory->goodbits[i] = -1;
		memory->size = newsize;
	}

	memory->goodbits[memory->max_index] = size;
	for (int i = memory->max_index + 1; i < memory->max_index + size; i++) memory->goodbits[i] = 0;
	memory->max_index += size;

	return where;
}

void memory_delete ( Memory *memory, int pointer) {
	if (pointer <= 0 || pointer >= memory->size) {
		exit (1);
	}
	if (memory->goodbits[pointer] < 0) {
		exit (1);
	}
	if (memory->goodbits[pointer] == 0) {
		exit (1);
	}
	int cnt = memory->goodbits[pointer];
	for (int i = pointer; i < pointer + cnt; i++) memory->goodbits[i] = -2;
}

void memory_set (Memory *memory, int pointer, int value) {
	if (pointer <= 0 || pointer >= memory->size) {
		exit (1);
	}
	if (memory->goodbits[pointer] < 0) {
		exit (1);
	}
	memory->memory[pointer] = value;
}

int  memory_get (Memory *memory, int pointer) {
	if (pointer <= 0 || pointer >= memory->size) {
		exit (1);
	}
	if (memory->goodbits[pointer] < 0) {
		exit (1);
	}
	return memory->memory[pointer];
}

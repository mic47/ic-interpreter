#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "error.h"

#define MIN_MEMORY_SIZE 10

void memory_init (Memory *memory) {
	memory->memory = (int*) malloc (sizeof (int) * MIN_MEMORY_SIZE);
	memory->goodbits = (int*) malloc (sizeof (int) * MIN_MEMORY_SIZE);
	for (int i = 0; i < MIN_MEMORY_SIZE; i++) memory->goodbits[i] = -1;
	memory->size = MIN_MEMORY_SIZE;
	memory->max_index = 0;
	memory->error_type = ERROR_USER;
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
		ERROR(memory->error_type,"If you want me to allocate %d words of memory, you must be fool.\n",size);
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
	if (pointer < 0 || pointer >= memory->size) {
		ERROR(memory->error_type,"Memory at %d is not even allocated!\n",pointer);
	}
	if(memory->goodbits[pointer]==-1)
		ERROR(memory->error_type,"Memory at %d is not allocated!\n",pointer);
	if(memory->goodbits[pointer]==-2)
		ERROR(memory->error_type,"Memory at %d is already free!\n",pointer);
	if(memory->goodbits[pointer]<-2)
		ERROR(ERROR_INTERNAL,"Memory at %d has strange state!\n",pointer);
	if (memory->goodbits[pointer] == 0) {
		ERROR(memory->error_type,"Memory at %d is part of larger chunk. I can free only whole chunk!\n",pointer);
	}
	int cnt = memory->goodbits[pointer];
	for (int i = pointer; i < pointer + cnt; i++) memory->goodbits[i] = -2;
}

void memory_set (Memory *memory, int pointer, int value) {
	if (pointer < 0 || pointer >= memory->size) {
		ERROR(memory->error_type,"Memory at %d is not allocated!\n",pointer);
	}
	if(memory->goodbits[pointer]==-1)
		ERROR(memory->error_type,"Memory at %d is not allocated!\n",pointer);
	if(memory->goodbits[pointer]==-2)
		ERROR(memory->error_type,"Memory at %d is already free!\n",pointer);
	if(memory->goodbits[pointer]<-2)
		ERROR(ERROR_INTERNAL,"Memory at %d has strange state!\n",pointer);
	memory->memory[pointer] = value;
}

int  memory_get (Memory *memory, int pointer) {
	if (pointer < 0 || pointer >= memory->size) {
		ERROR(memory->error_type,"Memory at %d is not allocated!\n",pointer);
	}
	if(memory->goodbits[pointer]==-1)
		ERROR(memory->error_type,"Memory at %d is not allocated!\n",pointer);
	if(memory->goodbits[pointer]==-2)
		ERROR(memory->error_type,"Memory at %d is already free!\n",pointer);
	if(memory->goodbits[pointer]<-2)
		ERROR(ERROR_INTERNAL,"Memory at %d has strange state!\n",pointer);
	return memory->memory[pointer];
}

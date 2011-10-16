#ifndef MEMORY_H
#define MEMORY_H

struct _Memory{
	int *memory;
	int *goodbits;
	int max_index;
	int size;
	int error_type;
};

typedef struct _Memory Memory;

int reuseMemory;
int randomMemory;


void memory_init(Memory *memory);
void memory_destroy(Memory *memory);
int  memory_new(Memory *memory,int size);
void memory_delete(Memory *memory,int pointer);
void memory_set(Memory *memory,int pointer, int value);
int  memory_get(Memory *memory,int pointer);

#endif

#ifndef BYTECODE_H
#define BYTECODE_H

#include "memory.h"

struct _Parameter{
	int type;
	int value;
};
typedef struct _Parameter Parameter;

struct _Instruction{
	int type;
	Parameter *param;
	int param_len;
	char *label;
};
typedef struct _Instruction Instruction;


struct _Program{
	int len;
	int ip;
	Instruction *instructions;
	Memory memory;
};

typedef struct _Program Program;

#endif

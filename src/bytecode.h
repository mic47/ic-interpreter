#ifndef BYTECODE_H
#define BYTECODE_H

struct Parameter{
	int type;
	int value;
};

struct Instruction{
	int type;
	Parameter *param;
};

struct Memory{
	int *cells;
	int min_cell;
	int max_cell;
	Stack stack;
};

struct Program{
	int len;
	int ip;
	Instruction *instructions;
};

#endif

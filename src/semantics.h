#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "bytecode.h"


#define SEM_V 0
#define SEM_P 1
#define SEM_C 2 
#define SEM_F 3
#define SEM_L 4
#define SEM_O 5
#define SEM_S 6

struct _Identifier{
	int type;
	char *text;
	int num;
};
typedef struct _Identifier Identifier;

struct _Programpiece {
	int len;
	int capacity;
	Instruction *instructions;
};
typedef struct _Programpiece Programpiece;

#endif

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define VAR_V 0
#define VAR_P 1
#define VAR_C 2

#define OP_PLUS 0
#define OP_MINUS 1
#define OP_MULTIPLY 2
#define OP_DIVIDE 3
#define OP_MODULO 4
#define OP_AND 5
#define OP_OR 6
#define OP_XOR 7
#define OP_SHL 8
#define OP_SHR 9

#define IN_NEW 		0
#define IN_DELETE 	1
#define IN_POP    	2
#define IN_RETURN 	3
#define IN_JUMP 	4
#define IN_READINT 	5
#define IN_WRITEINT 	6
#define IN_READCHAR 	7
#define IN_WRITECHAR 	8
#define IN_TOP 		9
#define IN_PUSH 	10
#define IN_CALL 	11
#define IN_OPERATOR 	12
#define IN_IFG 		13
#define IN_IFEQ 	14
#define IN_IFL 		15
#define IN_IFGEQ  	16
#define IN_IFLEQ 	17

#endif
